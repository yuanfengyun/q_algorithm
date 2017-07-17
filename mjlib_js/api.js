"use strict";

exports.MTableMgr = require( './table_mgr.js' );
exports.MHulib = require( './hulib.js' );

exports.Init = function()
{
	console.log("start  production...");
    this.MTableMgr.Init();
    console.log("end  production");
};