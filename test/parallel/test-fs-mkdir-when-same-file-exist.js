'use strict';

const common = require('../common');
const assert = require('assert');
const fs = require('fs');

fs.writeFileSync('./foo', '');

fs.mkdir('./foo/foo', { recursive: true }, function (e) {
  assert(e.code, 'EEXIST');
});
