<?php

//////////////////////////////////////////////////////////////////////////////
// PHP NationItem Class

class NationItem {
//////////////////////////////////////////////////////////////////////////////

public $Id        ;
public $Uuid      ;
public $Locality  ;
public $Priority  ;
public $Relevance ;
public $Flags     ;
public $Length    ;
public $Name      ;
public $Columns   ;

//////////////////////////////////////////////////////////////////////////////

function __construct()
{
  $this -> clear ( )  ;
}

function __destruct()
{
  unset ( $this -> Columns ) ;
}

public function clear()
{
  $this -> Id        = 0         ;
  $this -> Uuid      = 0         ;
  $this -> Locality  = 0         ;
  $this -> Priority  = 0         ;
  $this -> Relevance = 0         ;
  $this -> Flags     = 0         ;
  $this -> Length    = 0         ;
  $this -> Name      = ""        ;
  $this -> Columns   = array ( ) ;
}

public function assign($Item)
{
  $this -> Id        = $Item -> Id        ;
  $this -> Uuid      = $Item -> Uuid      ;
  $this -> Locality  = $Item -> Locality  ;
  $this -> Priority  = $Item -> Priority  ;
  $this -> Relevance = $Item -> Relevance ;
  $this -> Flags     = $Item -> Flags     ;
  $this -> Length    = $Item -> Length    ;
  $this -> Name      = $Item -> Name      ;
}

public function tableItems()
{
  $S = array (                  ) ;
  array_push ( $S , "id"        ) ;
  array_push ( $S , "uuid"      ) ;
  array_push ( $S , "locality"  ) ;
  array_push ( $S , "priority"  ) ;
  array_push ( $S , "relevance" ) ;
  array_push ( $S , "flags"     ) ;
  array_push ( $S , "length"    ) ;
  array_push ( $S , "name"      ) ;
  return $S                       ;
}

public function JoinItems ( $X , $S = "," )
{
  $U = array ( )               ;
  foreach ( $X as $V )         {
    $W = "`" . $V . "`"        ;
    array_push ( $U , $W )     ;
  }                            ;
  $L = implode ( $S , $U )     ;
  unset ( $U )                 ;
  return $L                    ;
}

public function Items( $S = "," )
{
  $X = $this -> tableItems (         ) ;
  $L = $this -> JoinItems  ( $X , $S ) ;
  unset                    ( $X      ) ;
  return $L                            ;
}

public function ClearColumns()
{
  unset ( $this -> Columns )     ;
  $this -> Columns   = array ( ) ;
}

public function AddColumn ( $C )
{
  array_push ( $this -> Columns , $C ) ;
}

public function isFlag($Mask)
{
  return ( ( gmp_and ( $Mask , $this -> Flags ) ) == $Mask ) ;
}

public function isUuid($u)
{
  return ( 0 == gmp_cmp ( $u , $this -> Uuid ) ) ;
}

public function isLocality($L)
{
  return ( $L == $this -> Locality ) ;
}

public function isRelevance ($r)
{
  return ( $r == $this -> Relevance ) ;
}

public function hasName()
{
  return ( strlen ( $this -> Name ) > 0 ) ;
}

public function set($item,$V)
{
  $a = strtolower ( $item )                        ;
  if ( "id"        == $a ) $this -> Id        = $V ;
  if ( "uuid"      == $a ) $this -> Uuid      = $V ;
  if ( "locality"  == $a ) $this -> Locality  = $V ;
  if ( "priority"  == $a ) $this -> Priority  = $V ;
  if ( "relevance" == $a ) $this -> Relevance = $V ;
  if ( "flags"     == $a ) $this -> Flags     = $V ;
  if ( "length"    == $a ) $this -> Length    = $V ;
  if ( "name"      == $a ) $this -> Name      = $V ;
}

public function setRelevance($N)
{
  global $NameUsages                      ;
  $this -> Relevance = $NameUsages [ $N ] ;
}

public function OptionsTail($Options,$Limits)
{
  $Q = ""                        ;
  if ( strlen ( $Options ) > 0 ) {
    $Q .= " "                    ;
    $Q .= $Options               ;
  }                              ;
  if ( strlen ( $Limits  ) > 0 ) {
    $Q .= " "                    ;
    $Q .= $Limits                ;
  }                              ;
  return $Q                      ;
}

public function ItemPair($item)
{
  $a = strtolower ( $item )                                ;
  if ( "id"        == $a )                                 {
    return "`" . $a . "` = " . (string) $this -> Id        ;
  }                                                        ;
  if ( "uuid"      == $a )                                 {
    return "`" . $a . "` = " . (string) $this -> Uuid      ;
  }                                                        ;
  if ( "locality"  == $a )                                 {
    return "`" . $a . "` = " . (string) $this -> Locality  ;
  }                                                        ;
  if ( "priority"  == $a )                                 {
    return "`" . $a . "` = " . (string) $this -> Priority  ;
  }                                                        ;
  if ( "relevance" == $a )                                 {
    return "`" . $a . "` = " . (string) $this -> Relevance ;
  }                                                        ;
  if ( "flags"     == $a )                                 {
    return "`" . $a . "` = " . (string) $this -> Flags     ;
  }                                                        ;
  if ( "length"    == $a )                                 {
    return "`" . $a . "` = " . (string) $this -> Length    ;
  }                                                        ;
  if ( "name"      == $a )                                 {
    return "`" . $a . "` = " . (string) $this -> Name      ;
  }                                                        ;
  return ""                                                ;
}

public function ItemPairs($items)
{
  $I = array ( )                                 ;
  foreach ( $items as $i )                       {
    array_push ( $I , $this -> ItemPair ( $i ) ) ;
  }                                              ;
  $L = implode ( " and " , $I )                  ;
  unset        (           $I )                  ;
  return $L                                      ;
}

public function QueryItems($items,$Options = "",$Limits = "")
{
  $Q = " where " . $this -> ItemPairs ( $items )        ;
  $Q = $Q . $this -> OptionsTail ( $Options , $Limits ) ;
  return $Q                                             ;
}

public function SelectItems($Table,$items,$Options = "",$Limits = "")
{
  return "select " . $this -> Items ( ) . " from " . $Table         .
         $this -> QueryItems ( $items , $Options , $Limits ) . " ;" ;
}

public function SelectColumns($Table,$Options = "order by `priority` asc",$Limits = "")
{
  return $this -> SelectItems ( $Table           ,
                                $this -> Columns ,
                                $Options         ,
                                $Limits        ) ;
}

public function Select                                 (
                  $Table                               ,
                  $Options = "order by `priority` asc" ,
                  $Limits  = "limit 0,1"               )
{
  $L = array                (                                        ) ;
  array_push                ( $L , "uuid" , "locality" , "relevance" ) ;
  $Q = $this -> SelectItems ( $Table , $L , $Options , $Limits       ) ;
  unset                     ( $L                                     ) ;
  return $Q                                                            ;
}

public function SelectPosition($Table)
{
  $L = array                (             ) ;
  array_push                ( $L            ,
                              "uuid"        ,
                              "locality"    ,
                              "priority"    ,
                              "relevance" ) ;
  $Q = "select `id` from " . $Table         .
       $this -> QueryItems  ( $L ) . " ;"   ;
  unset                     ( $L          ) ;
  return $Q                                 ;
}

public function LastPriority($Table)
{
  $L = array                 (                          ) ;
  array_push                 ( $L                         ,
                               "uuid"                     ,
                               "locality"                 ,
                               "relevance"              ) ;
  $QQ = "select `priority` from " . $Table                .
         $this -> QueryItems ( $L                         ,
                               "order by `priority` desc" ,
                               "limit 0,1" ) . " ;"       ;
  unset                      ( $L                       ) ;
  return $QQ                                              ;
}

public function Insert($Table)
{
  return "insert into " . $Table              .
            " (`uuid`,"                       .
          "`locality`,"                       .
          "`priority`,"                       .
         "`relevance`,"                       .
             "`flags`,"                       .
              "`name`,"                       .
            "`length`)"                       .
            " values ("                       .
            (string) $this -> Uuid      . "," .
            (string) $this -> Locality  . "," .
            (string) $this -> Priority  . "," .
            (string) $this -> Relevance . "," .
            (string) $this -> Flags     . "," .
            "?,length(name)) ;"               ;
}

public function Delete($Table)
{
  $L  = array (                                                     ) ;
  array_push  ( $L , "uuid" , "locality" , "priority" , "relevance" ) ;
  $QQ = "delete from " . $Table                                       .
        $this -> QueryItems ( $L )                             . " ;" ;
  unset ( $L )                                                        ;
  return $QQ                                                          ;
}

public function DeleteId($Table)
{
  return "delete from {$Table} where `id` = {$this->Id} ;" ;
}

public function Update($Table)
{
  $L  = array (                                                     ) ;
  array_push  ( $L , "uuid" , "locality" , "priority" , "relevance" ) ;
  $QQ = "update " . $Table                                            .
        " set `name` = ? ,"                                           .
        " `flags` = " . $this -> Flags . " ,"                         .
        " `length` = length(name)"                                    .
        $this -> QueryItems ( $L )                             . " ;" ;
  unset ( $L )                                                        ;
  return $QQ                                                          ;
}

public function UpdateId($Table)
{
  return "update " . $Table                           .
         " set `name` = ? ,"                          .
             " `uuid` = " . $this -> Uuid . " ,"      .
         " `locality` = " . $this -> Locality . " ,"  .
         " `priority` = " . $this -> Priority . " ,"  .
        " `relevance` = " . $this -> Relevance . " ," .
            " `flags` = " . $this -> Flags . " ,"     .
           " `length` = length(`name`)"               .
         " where `id` = " . $this -> Id . " ;"        ;
}

public function obtain($R)
{
  $this -> Id        = $R [ "id"        ] ;
  $this -> Uuid      = $R [ "uuid"      ] ;
  $this -> Locality  = $R [ "locality"  ] ;
  $this -> Priority  = $R [ "priority"  ] ;
  $this -> Relevance = $R [ "relevance" ] ;
  $this -> Flags     = $R [ "flags"     ] ;
  $this -> Length    = $R [ "length"    ] ;
  $this -> Name      = $R [ "name"      ] ;
}

//////////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////////
?>
