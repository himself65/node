'use strict';

require('../common');

const assert = require('assert');
const { execSync } = require('child_process');

const echo = 'hello';
const command = `echo ${echo}`;

const { stdout, stderr, error } = execSync(
  command
);

assert.ifError(error);
assert.deepStrictEqual(stderr, Buffer.alloc(0));
assert.deepStrictEqual(stdout, echo);
