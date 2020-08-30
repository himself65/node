'use strict';

const common = require('../common');
if (!common.hasCrypto)
  common.skip('missing crypto');
const assert = require('assert');
const child_process = require('child_process');
const path = require('path');

process.env.NODE_DEBUG_NATIVE = 'http2';
process.env.NODE_DEBUG = 'http2';
const { stdout, stderr } = child_process.spawnSync(process.execPath, [
  path.resolve('../parallel/test-http2-ping.js')
], { encoding: 'utf8' });

assert.strictEqual(stdout.toString(), '');
console.log(stderr);
