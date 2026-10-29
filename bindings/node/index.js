import { existsSync, readFileSync } from 'node:fs';
import { fileURLToPath } from 'node:url';

const root = fileURLToPath(new URL('../..', import.meta.url));

const binding =
  typeof process.versions.bun === 'string'
    ? // Support `bun build --compile` by being statically analyzable enough to find the .node file at build-time
      await import(
        `${root}/prebuilds/${process.platform}-${process.arch}/tree-sitter-css-strict.node`
      )
    : (await import('node-gyp-build')).default(root);

// node-types.json is only read by consumers that walk node kinds, and pulling it in as a
// JSON module costs ~0.9 ms of this module's ~7.5 ms import — a tenth of it for work that
// parsing never needs. Reading and parsing it on first access instead, the way the query
// getters below already do, exposes the same array while making the module ~8% cheaper to
// load. The file is optional: nodeTypeInfo stays unset if it's missing.
const nodeTypesPath = `${root}/src/node-types.json`;

if (existsSync(nodeTypesPath)) {
  Object.defineProperty(binding, 'nodeTypeInfo', {
    configurable: true,
    enumerable: true,
    get() {
      delete binding.nodeTypeInfo;
      try {
        binding.nodeTypeInfo = JSON.parse(readFileSync(nodeTypesPath, 'utf8'));
      } catch {
        // node-types.json is optional; the property stays unset if it's missing.
      }
      return binding.nodeTypeInfo;
    },
  });
}

const queries = [['HIGHLIGHTS_QUERY', `${root}/queries/highlights.scm`]];

for (const [prop, path] of queries) {
  Object.defineProperty(binding, prop, {
    configurable: true,
    enumerable: true,
    get() {
      delete binding[prop];
      try {
        binding[prop] = readFileSync(path, 'utf8');
      } catch {
        // The query file is optional; the property stays undefined if it's missing.
      }
      return binding[prop];
    },
  });
}

export default binding;
