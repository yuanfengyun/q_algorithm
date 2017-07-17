"use strict";

const fs = require( 'fs' );

var TABLE = function()
{
    this.tbl = {};
};

module.exports = TABLE;

TABLE.prototype.init = function()
{
    this.tbl = {};
};

TABLE.prototype.check = function( key )
{
    return this.tbl[ key ] ? true : false;
};

TABLE.prototype.add = function( key ) 
{
    // console.log(key);
    this.tbl[ key ] = 1;
};

//创建牌型文件
TABLE.prototype.dump = function( name )
{
   
    var fWrite = fs.createWriteStream(__dirname + '/tbl/' + name);  
    var  c=0;
    for(let  p  in  this.tbl){
        console.log(p);
        fWrite.write(p);
        fWrite.write('\n');
        c++;
        // console.log("打印="+this.tbl[p]);
    }
    fWrite.end();
    console.log("Dump File,FileName="+name+";length="+c);
};

//加载牌型
TABLE.prototype.load = function( name )
{
    if( !fs.existsSync( __dirname + '/tbl/' + name ) )
    {
        console.log( "文件不存在" );
        return;
    }
    let  c=0;
    const d = fs.readFileSync( __dirname + '/tbl/' + name );
    const n = String( d ).split( '\n' );
    for( let i = 0; i < n.length; i ++ )
    {
        if( n[ i ] )
        {
            this.tbl[ n[ i ] ] = 1;
            c++;
        }
    }
};