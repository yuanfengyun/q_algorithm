"use strict";

exports.MTableMgr = require( './table_mgr.js' );
exports.MHulib = require( './hulib.js' );

exports.init = function()
{
    this.MTableMgr.Init();
};