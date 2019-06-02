'use strict';

const common = require('../common');
const assert = require('assert');
const fs = require('fs');

fs.writeFileSync('./foo', '');

try {
  fs.mkdirSync('./foo/foo');
} catch (e) {
  assert(e.code, 'EEXIST');
}
