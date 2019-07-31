'use strict';
const common = require('../common');
const assert = require('assert');
const tls = require('tls');
const fixtures = require('../common/fixtures');

const options = {
  key: fixtures.readKey('agent1-key.pem'),
  cert: fixtures.readKey('agent1-cert.pem')
};

tls.createServer(options, common.mustCall(() => {}))
  .listen(0, function () {
    const error = new Error('foo');
    tls.connect({
      port: this.address().port
    }, common.mustCall(() => {
      throw error;
    })).on('error', common.mustCall((e) => {
      assert.strictEqual(e, error);
    }))
  });
