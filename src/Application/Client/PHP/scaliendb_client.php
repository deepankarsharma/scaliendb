<?php

/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.31
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

// Try to load our extension if it's not already loaded.
if (!extension_loaded("scaliendb_client")) {
  if (strtolower(substr(PHP_OS, 0, 3)) === 'win') {
    if (!dl('php_scaliendb_client.dll')) return;
  } else {
    // PHP_SHLIB_SUFFIX is available as of PHP 4.3.0, for older PHP assume 'so'.
    // It gives 'dylib' on MacOS X which is for libraries, modules are 'so'.
    if (PHP_SHLIB_SUFFIX === 'PHP_SHLIB_SUFFIX' || PHP_SHLIB_SUFFIX === 'dylib') {
      if (!dl('scaliendb_client.so')) return;
    } else {
      if (!dl('scaliendb_client.'.PHP_SHLIB_SUFFIX)) return;
    }
  }
}



abstract class scaliendb_client {
	static function imaxabs($n) {
		return imaxabs($n);
	}

	static function imaxdiv($numer,$denom) {
		$r=imaxdiv($numer,$denom);
		return is_resource($r) ? new imaxdiv_t($r) : $r;
	}

	static function strtoimax($nptr,$endptr,$base) {
		return strtoimax($nptr,$endptr,$base);
	}

	static function strtoumax($nptr,$endptr,$base) {
		return strtoumax($nptr,$endptr,$base);
	}

	static function SDBP_ResultClose($result) {
		SDBP_ResultClose($result);
	}

	static function SDBP_ResultKey($result) {
		return SDBP_ResultKey($result);
	}

	static function SDBP_ResultValue($result) {
		return SDBP_ResultValue($result);
	}

	static function SDBP_ResultKeyBuffer($result) {
		$r=SDBP_ResultKeyBuffer($result);
		return is_resource($r) ? new SDBP_Buffer($r) : $r;
	}

	static function SDBP_ResultValueBuffer($result) {
		$r=SDBP_ResultValueBuffer($result);
		return is_resource($r) ? new SDBP_Buffer($r) : $r;
	}

	static function SDBP_ResultSignedNumber($result) {
		return SDBP_ResultSignedNumber($result);
	}

	static function SDBP_ResultNumber($result) {
		return SDBP_ResultNumber($result);
	}

	static function SDBP_ResultIsConditionalSuccess($result) {
		return SDBP_ResultIsConditionalSuccess($result);
	}

	static function SDBP_ResultDatabaseID($result) {
		return SDBP_ResultDatabaseID($result);
	}

	static function SDBP_ResultTableID($result) {
		return SDBP_ResultTableID($result);
	}

	static function SDBP_ResultBegin($result) {
		SDBP_ResultBegin($result);
	}

	static function SDBP_ResultNext($result) {
		SDBP_ResultNext($result);
	}

	static function SDBP_ResultIsEnd($result) {
		return SDBP_ResultIsEnd($result);
	}

	static function SDBP_ResultIsFinished($result) {
		return SDBP_ResultIsFinished($result);
	}

	static function SDBP_ResultTransportStatus($result) {
		return SDBP_ResultTransportStatus($result);
	}

	static function SDBP_ResultCommandStatus($result) {
		return SDBP_ResultCommandStatus($result);
	}

	static function SDBP_ResultNumNodes($result) {
		return SDBP_ResultNumNodes($result);
	}

	static function SDBP_ResultNodeID($result,$n) {
		return SDBP_ResultNodeID($result,$n);
	}

	static function SDBP_ResultElapsedTime($result) {
		return SDBP_ResultElapsedTime($result);
	}

	static function SDBP_Create() {
		return SDBP_Create();
	}

	static function SDBP_Init($client,$params) {
		return SDBP_Init($client,$params);
	}

	static function SDBP_Destroy($client) {
		SDBP_Destroy($client);
	}

	static function SDBP_GetResult($client) {
		return SDBP_GetResult($client);
	}

	static function SDBP_SetGlobalTimeout($client,$timeout) {
		SDBP_SetGlobalTimeout($client,$timeout);
	}

	static function SDBP_SetMasterTimeout($client,$timeout) {
		SDBP_SetMasterTimeout($client,$timeout);
	}

	static function SDBP_GetGlobalTimeout($client) {
		return SDBP_GetGlobalTimeout($client);
	}

	static function SDBP_GetMasterTimeout($client) {
		return SDBP_GetMasterTimeout($client);
	}

	static function SDBP_GetCurrentDatabaseID($client) {
		return SDBP_GetCurrentDatabaseID($client);
	}

	static function SDBP_GetCurrentTableID($client) {
		return SDBP_GetCurrentTableID($client);
	}

	static function SDBP_GetJSONConfigState($client) {
		return SDBP_GetJSONConfigState($client);
	}

	static function SDBP_WaitConfigState($client) {
		SDBP_WaitConfigState($client);
	}

	static function SDBP_SetBatchLimit($client,$limit) {
		SDBP_SetBatchLimit($client,$limit);
	}

	static function SDBP_SetBulkLoading($client,$bulk) {
		SDBP_SetBulkLoading($client,$bulk);
	}

	static function SDBP_SetConsistencyLevel($client,$consistencyLevel) {
		SDBP_SetConsistencyLevel($client,$consistencyLevel);
	}

	static function SDBP_CreateQuorum($client,$params) {
		return SDBP_CreateQuorum($client,$params);
	}

	static function SDBP_DeleteQuorum($client,$quorumID) {
		return SDBP_DeleteQuorum($client,$quorumID);
	}

	static function SDBP_AddNode($client,$quorumID,$nodeID) {
		return SDBP_AddNode($client,$quorumID,$nodeID);
	}

	static function SDBP_RemoveNode($client,$quorumID,$nodeID) {
		return SDBP_RemoveNode($client,$quorumID,$nodeID);
	}

	static function SDBP_ActivateNode($client,$nodeID) {
		return SDBP_ActivateNode($client,$nodeID);
	}

	static function SDBP_CreateDatabase($client,$name) {
		return SDBP_CreateDatabase($client,$name);
	}

	static function SDBP_RenameDatabase($client,$databaseID,$name) {
		return SDBP_RenameDatabase($client,$databaseID,$name);
	}

	static function SDBP_DeleteDatabase($client,$databaseID) {
		return SDBP_DeleteDatabase($client,$databaseID);
	}

	static function SDBP_CreateTable($client,$databaseID,$quorumID,$name) {
		return SDBP_CreateTable($client,$databaseID,$quorumID,$name);
	}

	static function SDBP_RenameTable($client,$tableID,$name) {
		return SDBP_RenameTable($client,$tableID,$name);
	}

	static function SDBP_DeleteTable($client,$tableID) {
		return SDBP_DeleteTable($client,$tableID);
	}

	static function SDBP_TruncateTable($client,$tableID) {
		return SDBP_TruncateTable($client,$tableID);
	}

	static function SDBP_SplitShard($client,$shardID,$key) {
		return SDBP_SplitShard($client,$shardID,$key);
	}

	static function SDBP_FreezeTable($client,$tableID) {
		return SDBP_FreezeTable($client,$tableID);
	}

	static function SDBP_UnfreezeTable($client,$tableID) {
		return SDBP_UnfreezeTable($client,$tableID);
	}

	static function SDBP_MigrateShard($client,$quorumID,$shardID) {
		return SDBP_MigrateShard($client,$quorumID,$shardID);
	}

	static function SDBP_GetDatabaseID($client,$name) {
		return SDBP_GetDatabaseID($client,$name);
	}

	static function SDBP_GetTableID($client,$databaseID,$name) {
		return SDBP_GetTableID($client,$databaseID,$name);
	}

	static function SDBP_UseDatabase($client,$name) {
		return SDBP_UseDatabase($client,$name);
	}

	static function SDBP_UseDatabaseID($client,$databaseID) {
		return SDBP_UseDatabaseID($client,$databaseID);
	}

	static function SDBP_UseTable($client,$name) {
		return SDBP_UseTable($client,$name);
	}

	static function SDBP_UseTableID($client,$tableID) {
		return SDBP_UseTableID($client,$tableID);
	}

	static function SDBP_GetNumQuorums($client) {
		return SDBP_GetNumQuorums($client);
	}

	static function SDBP_GetQuorumIDAt($client,$n) {
		return SDBP_GetQuorumIDAt($client,$n);
	}

	static function SDBP_GetNumDatabases($client) {
		return SDBP_GetNumDatabases($client);
	}

	static function SDBP_GetDatabaseIDAt($client,$n) {
		return SDBP_GetDatabaseIDAt($client,$n);
	}

	static function SDBP_GetDatabaseNameAt($client,$n) {
		return SDBP_GetDatabaseNameAt($client,$n);
	}

	static function SDBP_GetNumTables($client) {
		return SDBP_GetNumTables($client);
	}

	static function SDBP_GetTableIDAt($client,$n) {
		return SDBP_GetTableIDAt($client,$n);
	}

	static function SDBP_GetTableNameAt($client,$n) {
		return SDBP_GetTableNameAt($client,$n);
	}

	static function SDBP_Get($client,$key) {
		return SDBP_Get($client,$key);
	}

	static function SDBP_GetCStr($client,$key,$len) {
		return SDBP_GetCStr($client,$key,$len);
	}

	static function SDBP_Set($client,$key,$value) {
		return SDBP_Set($client,$key,$value);
	}

	static function SDBP_SetCStr($client_,$key,$lenKey,$value,$lenValue) {
		return SDBP_SetCStr($client_,$key,$lenKey,$value,$lenValue);
	}

	static function SDBP_SetIfNotExists($client,$key,$value) {
		return SDBP_SetIfNotExists($client,$key,$value);
	}

	static function SDBP_SetIfNotExistsCStr($client,$key,$lenKey,$value,$lenValue) {
		return SDBP_SetIfNotExistsCStr($client,$key,$lenKey,$value,$lenValue);
	}

	static function SDBP_TestAndSet($client,$key,$test,$value) {
		return SDBP_TestAndSet($client,$key,$test,$value);
	}

	static function SDBP_TestAndSetCStr($client,$key,$lenKey,$test,$lenTest,$value,$lenValue) {
		return SDBP_TestAndSetCStr($client,$key,$lenKey,$test,$lenTest,$value,$lenValue);
	}

	static function SDBP_GetAndSet($client,$key,$value) {
		return SDBP_GetAndSet($client,$key,$value);
	}

	static function SDBP_GetAndSetCStr($client,$key,$lenKey,$value,$lenValue) {
		return SDBP_GetAndSetCStr($client,$key,$lenKey,$value,$lenValue);
	}

	static function SDBP_Add($client,$key,$number) {
		return SDBP_Add($client,$key,$number);
	}

	static function SDBP_AddCStr($client_,$key,$len,$number) {
		return SDBP_AddCStr($client_,$key,$len,$number);
	}

	static function SDBP_Append($client,$key,$value) {
		return SDBP_Append($client,$key,$value);
	}

	static function SDBP_AppendCStr($client_,$key,$lenKey,$value,$lenValue) {
		return SDBP_AppendCStr($client_,$key,$lenKey,$value,$lenValue);
	}

	static function SDBP_Delete($client,$key) {
		return SDBP_Delete($client,$key);
	}

	static function SDBP_DeleteCStr($client_,$key,$len) {
		return SDBP_DeleteCStr($client_,$key,$len);
	}

	static function SDBP_TestAndDelete($client,$key,$test) {
		return SDBP_TestAndDelete($client,$key,$test);
	}

	static function SDBP_TestAndDeleteCStr($client_,$key,$keylen,$test,$testlen) {
		return SDBP_TestAndDeleteCStr($client_,$key,$keylen,$test,$testlen);
	}

	static function SDBP_Remove($client,$key) {
		return SDBP_Remove($client,$key);
	}

	static function SDBP_RemoveCStr($client_,$key,$len) {
		return SDBP_RemoveCStr($client_,$key,$len);
	}

	static function SDBP_ListKeys($client,$startKey,$endKey,$prefix,$count,$offset) {
		return SDBP_ListKeys($client,$startKey,$endKey,$prefix,$count,$offset);
	}

	static function SDBP_ListKeysCStr($client,$startKey,$startKeyLen,$endKey,$endKeyLen,$prefix,$prefixLen,$count,$offset) {
		return SDBP_ListKeysCStr($client,$startKey,$startKeyLen,$endKey,$endKeyLen,$prefix,$prefixLen,$count,$offset);
	}

	static function SDBP_ListKeyValues($client,$startKey,$endKey,$prefix,$count,$offset) {
		return SDBP_ListKeyValues($client,$startKey,$endKey,$prefix,$count,$offset);
	}

	static function SDBP_ListKeyValuesCStr($client,$startKey,$startKeyLen,$endKey,$endKeyLen,$prefix,$prefixLen,$count,$offset) {
		return SDBP_ListKeyValuesCStr($client,$startKey,$startKeyLen,$endKey,$endKeyLen,$prefix,$prefixLen,$count,$offset);
	}

	static function SDBP_Count($client,$startKey,$endKey,$prefix,$count,$offset) {
		return SDBP_Count($client,$startKey,$endKey,$prefix,$count,$offset);
	}

	static function SDBP_CountCStr($client,$startKey,$startKeyLen,$endKey,$endKeyLen,$prefix,$prefixLen,$count,$offset) {
		return SDBP_CountCStr($client,$startKey,$startKeyLen,$endKey,$endKeyLen,$prefix,$prefixLen,$count,$offset);
	}

	static function SDBP_Begin($client) {
		return SDBP_Begin($client);
	}

	static function SDBP_Submit($client) {
		return SDBP_Submit($client);
	}

	static function SDBP_Cancel($client) {
		return SDBP_Cancel($client);
	}

	static function SDBP_IsBatched($client) {
		return SDBP_IsBatched($client);
	}

	static function SDBP_SetTrace($trace) {
		SDBP_SetTrace($trace);
	}

	static function SDBP_GetVersion() {
		return SDBP_GetVersion();
	}

	static function SDBP_GetDebugString() {
		return SDBP_GetDebugString();
	}
}

/* PHP Proxy Classes */
class imaxdiv_t {
	public $_cPtr=null;

	function __set($var,$value) {
		if ($var == 'quot') return imaxdiv_t_quot_set($this->_cPtr,$value);
		if ($var == 'rem') return imaxdiv_t_rem_set($this->_cPtr,$value);
	}

	function __get($var) {
		if ($var == 'quot') return imaxdiv_t_quot_get($this->_cPtr);
		if ($var == 'rem') return imaxdiv_t_rem_get($this->_cPtr);
		return null;
	}

	function __construct() {
		$this->_cPtr=new_imaxdiv_t();
	}
}

class SDBP_NodeParams {
	public $_cPtr=null;

	function __set($var,$value) {
		$func = 'SDBP_NodeParams_'.$var.'_set';
		if (function_exists($func)) call_user_func($func,$this->_cPtr,$value);
	}

	function __get($var) {
		$func = 'SDBP_NodeParams_'.$var.'_get';
		if (function_exists($func)) return call_user_func($func,$this->_cPtr);
		return null;
	}

	function __construct($nodec_) {
		$this->_cPtr=new_SDBP_NodeParams($nodec_);
	}

	function Close() {
		SDBP_NodeParams_Close($this->_cPtr);
	}

	function AddNode($node) {
		SDBP_NodeParams_AddNode($this->_cPtr,$node);
	}
}

class SDBP_Buffer {
	public $_cPtr=null;

	function __set($var,$value) {
		if ($var == 'data') return SDBP_Buffer_data_set($this->_cPtr,$value);
		if ($var == 'len') return SDBP_Buffer_len_set($this->_cPtr,$value);
	}

	function __get($var) {
		if ($var == 'data') return SDBP_Buffer_data_get($this->_cPtr);
		if ($var == 'len') return SDBP_Buffer_len_get($this->_cPtr);
		return null;
	}

	function __construct() {
		$this->_cPtr=new_SDBP_Buffer();
	}

	function SetBuffer($data_,$len_) {
		SDBP_Buffer_SetBuffer($this->_cPtr,$data_,$len_);
	}
}


?>
