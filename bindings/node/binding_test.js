import assert from 'node:assert';
import { test } from 'node:test';
import Parser from 'tree-sitter';

test('can load grammar', async () => {
  const parser = new Parser();
  const { default: language } = await import('./index.js');
  assert.doesNotThrow(() => {
    parser.setLanguage(language);
  });
});

test('loads node-types.json on first access and keeps the result', async () => {
  const { default: language } = await import('./index.js');
  const info = language.nodeTypeInfo;
  assert.ok(Array.isArray(info));
  assert.ok(info.length > 0);
  assert.strictEqual(language.nodeTypeInfo, info);
});
