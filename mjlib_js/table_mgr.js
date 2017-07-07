"use strict";

let Table = require( './table.js' );

let TableMgr = module.exports;

TableMgr.m_tbl = {};
TableMgr.m_eye_tbl = {};
TableMgr.m_feng_tbl = {};
TableMgr.m_feng_eye_tbl = {};

TableMgr.Init = function()
{
    for ( let i =0; i < 9; i++ )
    {
        this.m_tbl[ i ] = new Table();
        this.m_tbl[ i ].init();
    }

    for ( let i =0; i < 9; i++ )
    {
        this.m_eye_tbl[ i ] = new Table();
        this.m_eye_tbl[ i ].init();
    }

    for ( let i =0; i < 9; i++ )
    {
        this.m_feng_tbl[ i ] = new Table();
        this.m_feng_tbl[ i ].init();
    }

    for ( let i =0; i < 9; i++ )
    {
        this.m_feng_eye_tbl[ i ] = new Table();
        this.m_feng_eye_tbl[ i ].init();
    }
};

TableMgr.getTable = function( gui_num, eye, chi )
{

    let tbl = null;
    if ( chi ) 
    {
        tbl = this.m_tbl[ gui_num ];
        if ( eye ) 
        {
            tbl = this.m_eye_tbl[ gui_num ];
        }
    } 
    else 
    {
        tbl = this.m_feng_tbl[ gui_num ];
        if ( eye ) 
        {
            tbl = this.m_feng_eye_tbl[ gui_num ];
        }
    }
    return tbl;
};

TableMgr.Add = function( key, gui_num, eye, chi) 
{
    let tbl = this.getTable( gui_num, eye, chi );
    if( tbl )
    {
        tbl.add( key );
    }
};

TableMgr.check = function( key, gui_num, eye, chi ) 
{
    let tbl = this.getTable( gui_num, eye, chi );

    if( !tbl ) return false;
    
    return tbl.check( key );
};

TableMgr.LoadTable = function()
{
    for( let i = 0; i < 9; i ++ )
    {
        let name = "table_%d.tbl".replace( '%d', i );
        this.m_tbl[ i ].load( name );
    }

    for( let i = 0; i < 9; i ++ )
    {
        let name = "eye_table_%d.tbl".replace( '%d', i );
        this.m_eye_tbl[ i ].load( name );
    }
};

TableMgr.DumpTable = function() 
{
    for ( let i = 0; i < 9; i++ )
    {
        let name = "table_%d.tbl".replace( '%d', i );
        this.m_tbl[ i ].dump( name );
    }
    for ( let i = 0; i < 9; i++ )
    {
        let name = "eye_table_%d.tbl".replace( '%d', i );
        this.m_eye_tbl[ i ].dump( name );
    }
};

TableMgr.DumpFengTable = function() 
{
    for ( let i = 0; i < 9; i++ )
    {
        let name = "feng_table_%d.tbl".replace( '%d', i );
        this.m_tbl[ i ].dump( name );
    }
    for ( let i = 0; i < 9; i++ )
    {
        let name = "feng_eye_table_%d.tbl".replace( '%d', i );
        this.m_tbl[ i ].dump( name );
    }
};