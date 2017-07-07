var fs = require( 'fs' );

var d = fs.readFileSync( __dirname + '/table_0.tbl' );
var n = String( d ).split( '\n' );

for( var i = 0; i < n.length; i ++ )
{
    console.log( '==>' + n[ i ] );
}
