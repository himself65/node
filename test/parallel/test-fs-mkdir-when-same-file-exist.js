'use strict';

require('../common');
const assert = require('assert');
const fs = require('fs');
const path = require('path');
const tmpdir = require('../common/tmpdir');
const file = path.join(tmpdir.path, 'foo');
const dir = path.join(file, './foo');

fs.writeFileSync(file, '');

fs.mkdir(dir, { recursive: true }, function(err) {
  assert.ok(/^EEXIST/.test(err.message), 'got no EEXIST message');
  assert.strictEqual(err.path, dir);
});

fs.mkdir(dir, { recursive: false }, function(err) {
  assert.ok(/^EEXIST/.test(err.message), 'got no EEXIST message');
  assert.strictEqual(err.path, dir);
});
