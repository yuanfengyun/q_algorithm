"use strict";

let MTableMgr = require( './table_mgr.js' );

const MAX_CARD = 34;
let ProbabilityItem = { };
let ProbabilityItemTable = { };

function _init()
{
    ProbabilityItem = { eye : false, gui_num : 0 };
    ProbabilityItemTable = { array_num : 0, m_num : [ 0, 0, 0, 0 ], m : [] };
    for( let i = 0; i < 4; i ++ )
    {
        ProbabilityItemTable.m[ i ] = [];
        for( let j = 0; j < 5; j ++ )
        {
            ProbabilityItemTable.m[ i ].push( ProbabilityItem );
        }
    }
}
_init();

let HuLib = module.exports;

HuLib.gui_index1 = 31;
HuLib.gui_index2 = 32;

HuLib.init = function()
{
    //初始化数据
    this.gui_index1 = 31;
    this.gui_index2 = 32;
    _init();
};

HuLib.get_hu_info = function( cards, cur_card )
{
	_init()
    let tmp_cards = cards.concat();
    if ( cur_card != MAX_CARD ) 
    {
        tmp_cards[ cur_card ] += 1;
    }
    let gui_index1 = this.gui_index1;
    let gui_index2 = this.gui_index2;
    let gui_num = 0;
    if( gui_index1 != MAX_CARD )
    {
        gui_num += tmp_cards[ gui_index1 ];
        tmp_cards[ gui_index1 ] = 0;

    }
    if( gui_index2 != MAX_CARD ) 
    {
        gui_num += parseInt( tmp_cards[ gui_index2 ] );
        tmp_cards[ gui_index2 ] = 0;
    }
    //var ptbl ProbabilityItemTable
    if ( this._split( tmp_cards, gui_num, ProbabilityItemTable ) )
    {
        return false;
    }
    return this.check_probability( ProbabilityItemTable, gui_num );
};

HuLib.check_7dui = function( cards, gui_num ) 
{
    let need = 0;
    for( let i = 1; i < 35; i++ ) 
    {
        if ( cards[ i ] % 2 != 0 ) 
        {
            need += 1;
        }
    }
    return need > gui_num ? false : true;
};

HuLib.check_probability = function( ptbl, gui_num ) 
{
    // 全是鬼牌
    if( ptbl.array_num == 0 )
    {
        return gui_num >= 2;
    }
    // 只有一种花色的牌的鬼牌
    if ( ptbl.array_num == 1 ) 
    {
        return true;
    }
    // 尝试组合花色，能组合则胡
    for ( let i = 0; i < ptbl.m_num[ 0 ]; i++ ) 
    {
        let item = ptbl.m[0][i];
        let eye = item.eye;
        let gui = gui_num - item.gui_num;
        if ( this.check_probability_sub( ptbl, eye, gui, 1, ptbl.array_num ) )
        {
            return true;
        }
    }
    return false;
};

HuLib.check_probability_sub = function( ptbl, eye, gui_num, level, max_level ) 
{
    for ( let i = 0; i < ptbl.m_num[ level ]; i++ ) 
    {
        let item = ptbl.m[ level ][ i ];
        if ( eye && item.eye ) 
        {
            continue;
        }
        if ( gui_num < item.gui_num ) 
        {
            continue;
        }
        if ( level < max_level - 1 ) 
        {
            let old_gui_num = gui_num;
            let old_eye = eye;
            gui_num -= item.gui_num;
            eye = eye || item.eye;
            if ( this.check_probability_sub( ptbl, eye, gui_num, level + 1, ptbl.array_num ) ) 
            {
                return true;
            }
            eye = old_eye;
            gui_num = old_gui_num;
            continue;

        }
        if ( !eye && !item.eye && item.gui_num + 2 > gui_num ) 
        {
            continue;
        }
        return true;
    }
};

HuLib._split = function( cards, gui_num, ptbl )
{

    if ( this._split_color( cards, gui_num, 0, 1, 9, true, ptbl ) )
    {
        return false;
    }
    if ( this._split_color( cards, gui_num, 1, 10, 18, true, ptbl ) ) 
    {
        return false;
    }
    if ( this._split_color(cards, gui_num, 2, 19, 27, true, ptbl ) )
    {
        return false;
    }
    if ( this._split_color(cards, gui_num, 3, 28, 34, false, ptbl ) ) 
    {
        return false;
    }
    return true;

}

HuLib._split_color = function( cards, gui_num, color, min, max, chi, ptbl )
{

    let key = 0;
    let num = 0;
    for( let i = min; i <= max; i++ ) 
    {
        key = key * 10 + cards[ i ];
        num = num + cards[ i ];
    }
    if ( num === 0 ) 
    {
        return true;
    }
    if ( this.list_probability( color, gui_num, num, key, chi, ptbl ) )
    {
        return false;
    }
    return true;
};

HuLib.list_probability = function( color, gui_num, num, key, chi, ptbl ) 
{
    let anum = ptbl.array_num;
    for ( let i = 0; i <= gui_num; i++ ) 
    {
        let eye = false;
        let yu = ( num + i ) % 3;
        if ( yu == 1 ) 
        {
            continue;
        }
        if( yu == 2 )
        {
            eye = true;
        }
        if ( MTableMgr.check( key, i, eye, chi ) )
        {
            let item = ptbl.m[ anum ][ ptbl.m_num[ anum ] ];
            ptbl.m_num[ anum ]++;
            item.eye = eye;
            item.gui_num = i;
        }
    }
    if( ptbl.m_num[ anum ] <= 0 ) 
    {
        return false;
    }
    ptbl.array_num++;
    return true;
};