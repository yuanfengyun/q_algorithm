"use strict";

exports.MTableMgr = require( './table_mgr.js' );
exports.MHulib = require( './hulib.js' );

exports.init = function()
{
	console.log("初始化...");
    this.MTableMgr.Init();
};