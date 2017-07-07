var api = require( './api.js' );
api.Init();
api.MTableMgr.LoadTable();
//var cards = [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 32, 32, 32, 25 ];
//var d = api.MHulib.get_hu_info( cards, 25 );
//console.log( d );

function print_cards( cards ) 
{

    for( var i = 0; i<9; i++ )
    {

        console.log("%d,", cards[i])

    }

    console.log("\n")



    for ( var i = 9; i<18; i++ )
    {

        console.log("%d,", cards[i])

    }

    console.log("\n");



    for ( var i =18; i<27; i++ )
    {

        console.log("%d,", cards[i])

    }

    console.log("\n");



    for ( var i =27; i<34; i++ )
    {

        console.log("%d,", cards[i])

    }

    console.log("\n");

}

function tt(arr)
{
    var t = {};
    for( var i = 0; i < arr.length; i ++ )
    {
        if( !t[ arr[ i ] ] ) t[ arr[ i ] ] = 0;

        t[ arr[ i ] ] += 1;
    }
    var ret = [];
    for( var i = 1; i < 35; i ++ )
    {
        ret.push( t[ i ] ? t[ i ] : 0 );
    }
    return ret;
}

function test_one_success() {

    var cards = [
        0,0,0,0,0,0,0,0,0,
        1,1,1,2,3,0,0,0,0,
        0,0,0,2,2,2,0,0,0,
        0,0,0,0,0,0,0 ];
    console.log("测试1种能胡的牌型")

    //print_cards(cards);
    var cards = [ 1, 2, 3, 33, 33, 33, 7, 8, 9, 32, 32, 32, 25 ];
    var al = tt(    cards );
    var t = Date.now();
    var ret = api.MHulib.get_hu_info( al, 33 );
    console.log( ret );
    console.log( 'ttime=>' + ( Date.now() - t ) );

}
test_one_success();