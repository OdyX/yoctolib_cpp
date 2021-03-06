/*********************************************************************
 *
 * $Id: yocto_weighscale.cpp 28231 2017-07-31 16:37:33Z mvuilleu $
 *
 * Implements yFindWeighScale(), the high-level API for WeighScale functions
 *
 * - - - - - - - - - License information: - - - - - - - - - 
 *
 *  Copyright (C) 2011 and beyond by Yoctopuce Sarl, Switzerland.
 *
 *  Yoctopuce Sarl (hereafter Licensor) grants to you a perpetual
 *  non-exclusive license to use, modify, copy and integrate this
 *  file into your software for the sole purpose of interfacing
 *  with Yoctopuce products.
 *
 *  You may reproduce and distribute copies of this file in
 *  source or object form, as long as the sole purpose of this
 *  code is to interface with Yoctopuce products. You must retain
 *  this notice in the distributed source file.
 *
 *  You should refer to Yoctopuce General Terms and Conditions
 *  for additional information regarding your rights and
 *  obligations.
 *
 *  THE SOFTWARE AND DOCUMENTATION ARE PROVIDED 'AS IS' WITHOUT
 *  WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING 
 *  WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS
 *  FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO
 *  EVENT SHALL LICENSOR BE LIABLE FOR ANY INCIDENTAL, SPECIAL,
 *  INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 *  COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR
 *  SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT
 *  LIMITED TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR
 *  CONTRIBUTION, OR OTHER SIMILAR COSTS, WHETHER ASSERTED ON THE
 *  BASIS OF CONTRACT, TORT (INCLUDING NEGLIGENCE), BREACH OF
 *  WARRANTY, OR OTHERWISE.
 *
 *********************************************************************/


#define _CRT_SECURE_NO_DEPRECATE //do not use windows secure crt
#include "yocto_weighscale.h"
#include "yapi/yjson.h"
#include "yapi/yapi.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define  __FILE_ID__  "weighscale"

YWeighScale::YWeighScale(const string& func): YSensor(func)
//--- (WeighScale initialization)
    ,_excitation(EXCITATION_INVALID)
    ,_adaptRatio(ADAPTRATIO_INVALID)
    ,_compTemperature(COMPTEMPERATURE_INVALID)
    ,_compensation(COMPENSATION_INVALID)
    ,_zeroTracking(ZEROTRACKING_INVALID)
    ,_command(COMMAND_INVALID)
    ,_valueCallbackWeighScale(NULL)
    ,_timedReportCallbackWeighScale(NULL)
//--- (end of WeighScale initialization)
{
    _className="WeighScale";
}

YWeighScale::~YWeighScale()
{
//--- (YWeighScale cleanup)
//--- (end of YWeighScale cleanup)
}
//--- (YWeighScale implementation)
// static attributes
const double YWeighScale::ADAPTRATIO_INVALID = YAPI_INVALID_DOUBLE;
const double YWeighScale::COMPTEMPERATURE_INVALID = YAPI_INVALID_DOUBLE;
const double YWeighScale::COMPENSATION_INVALID = YAPI_INVALID_DOUBLE;
const double YWeighScale::ZEROTRACKING_INVALID = YAPI_INVALID_DOUBLE;
const string YWeighScale::COMMAND_INVALID = YAPI_INVALID_STRING;

int YWeighScale::_parseAttr(YJSONObject* json_val)
{
    if(json_val->has("excitation")) {
        _excitation =  (Y_EXCITATION_enum)json_val->getInt("excitation");
    }
    if(json_val->has("adaptRatio")) {
        _adaptRatio =  floor(json_val->getDouble("adaptRatio") * 1000.0 / 65536.0 + 0.5) / 1000.0;
    }
    if(json_val->has("compTemperature")) {
        _compTemperature =  floor(json_val->getDouble("compTemperature") * 1000.0 / 65536.0 + 0.5) / 1000.0;
    }
    if(json_val->has("compensation")) {
        _compensation =  floor(json_val->getDouble("compensation") * 1000.0 / 65536.0 + 0.5) / 1000.0;
    }
    if(json_val->has("zeroTracking")) {
        _zeroTracking =  floor(json_val->getDouble("zeroTracking") * 1000.0 / 65536.0 + 0.5) / 1000.0;
    }
    if(json_val->has("command")) {
        _command =  json_val->getString("command");
    }
    return YSensor::_parseAttr(json_val);
}


/**
 * Returns the current load cell bridge excitation method.
 *
 * @return a value among Y_EXCITATION_OFF, Y_EXCITATION_DC and Y_EXCITATION_AC corresponding to the
 * current load cell bridge excitation method
 *
 * On failure, throws an exception or returns Y_EXCITATION_INVALID.
 */
Y_EXCITATION_enum YWeighScale::get_excitation(void)
{
    Y_EXCITATION_enum res;
    yEnterCriticalSection(&_this_cs);
    try {
        if (_cacheExpiration <= YAPI::GetTickCount()) {
            if (this->_load_unsafe(YAPI::DefaultCacheValidity) != YAPI_SUCCESS) {
                {
                    yLeaveCriticalSection(&_this_cs);
                    return YWeighScale::EXCITATION_INVALID;
                }
            }
        }
        res = _excitation;
    } catch (std::exception) {
        yLeaveCriticalSection(&_this_cs);
        throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

/**
 * Changes the current load cell bridge excitation method.
 *
 * @param newval : a value among Y_EXCITATION_OFF, Y_EXCITATION_DC and Y_EXCITATION_AC corresponding
 * to the current load cell bridge excitation method
 *
 * @return YAPI_SUCCESS if the call succeeds.
 *
 * On failure, throws an exception or returns a negative error code.
 */
int YWeighScale::set_excitation(Y_EXCITATION_enum newval)
{
    string rest_val;
    int res;
    yEnterCriticalSection(&_this_cs);
    try {
        char buf[32]; sprintf(buf, "%d", newval); rest_val = string(buf);
        res = _setAttr("excitation", rest_val);
    } catch (std::exception) {
         yLeaveCriticalSection(&_this_cs);
         throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

/**
 * Changes the compensation temperature update rate, in percents.
 *
 * @param newval : a floating point number corresponding to the compensation temperature update rate, in percents
 *
 * @return YAPI_SUCCESS if the call succeeds.
 *
 * On failure, throws an exception or returns a negative error code.
 */
int YWeighScale::set_adaptRatio(double newval)
{
    string rest_val;
    int res;
    yEnterCriticalSection(&_this_cs);
    try {
        char buf[32]; sprintf(buf,"%d", (int)floor(newval * 65536.0 + 0.5)); rest_val = string(buf);
        res = _setAttr("adaptRatio", rest_val);
    } catch (std::exception) {
         yLeaveCriticalSection(&_this_cs);
         throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

/**
 * Returns the compensation temperature update rate, in percents.
 * the maximal value is 65 percents.
 *
 * @return a floating point number corresponding to the compensation temperature update rate, in percents
 *
 * On failure, throws an exception or returns Y_ADAPTRATIO_INVALID.
 */
double YWeighScale::get_adaptRatio(void)
{
    double res = 0.0;
    yEnterCriticalSection(&_this_cs);
    try {
        if (_cacheExpiration <= YAPI::GetTickCount()) {
            if (this->_load_unsafe(YAPI::DefaultCacheValidity) != YAPI_SUCCESS) {
                {
                    yLeaveCriticalSection(&_this_cs);
                    return YWeighScale::ADAPTRATIO_INVALID;
                }
            }
        }
        res = _adaptRatio;
    } catch (std::exception) {
        yLeaveCriticalSection(&_this_cs);
        throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

/**
 * Returns the current compensation temperature.
 *
 * @return a floating point number corresponding to the current compensation temperature
 *
 * On failure, throws an exception or returns Y_COMPTEMPERATURE_INVALID.
 */
double YWeighScale::get_compTemperature(void)
{
    double res = 0.0;
    yEnterCriticalSection(&_this_cs);
    try {
        if (_cacheExpiration <= YAPI::GetTickCount()) {
            if (this->_load_unsafe(YAPI::DefaultCacheValidity) != YAPI_SUCCESS) {
                {
                    yLeaveCriticalSection(&_this_cs);
                    return YWeighScale::COMPTEMPERATURE_INVALID;
                }
            }
        }
        res = _compTemperature;
    } catch (std::exception) {
        yLeaveCriticalSection(&_this_cs);
        throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

/**
 * Returns the current current thermal compensation value.
 *
 * @return a floating point number corresponding to the current current thermal compensation value
 *
 * On failure, throws an exception or returns Y_COMPENSATION_INVALID.
 */
double YWeighScale::get_compensation(void)
{
    double res = 0.0;
    yEnterCriticalSection(&_this_cs);
    try {
        if (_cacheExpiration <= YAPI::GetTickCount()) {
            if (this->_load_unsafe(YAPI::DefaultCacheValidity) != YAPI_SUCCESS) {
                {
                    yLeaveCriticalSection(&_this_cs);
                    return YWeighScale::COMPENSATION_INVALID;
                }
            }
        }
        res = _compensation;
    } catch (std::exception) {
        yLeaveCriticalSection(&_this_cs);
        throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

/**
 * Changes the compensation temperature update rate, in percents.
 *
 * @param newval : a floating point number corresponding to the compensation temperature update rate, in percents
 *
 * @return YAPI_SUCCESS if the call succeeds.
 *
 * On failure, throws an exception or returns a negative error code.
 */
int YWeighScale::set_zeroTracking(double newval)
{
    string rest_val;
    int res;
    yEnterCriticalSection(&_this_cs);
    try {
        char buf[32]; sprintf(buf,"%d", (int)floor(newval * 65536.0 + 0.5)); rest_val = string(buf);
        res = _setAttr("zeroTracking", rest_val);
    } catch (std::exception) {
         yLeaveCriticalSection(&_this_cs);
         throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

/**
 * Returns the zero tracking threshold value. When this threshold is larger than
 * zero, any measure under the threshold will automatically be ignored and the
 * zero compensation will be updated.
 *
 * @return a floating point number corresponding to the zero tracking threshold value
 *
 * On failure, throws an exception or returns Y_ZEROTRACKING_INVALID.
 */
double YWeighScale::get_zeroTracking(void)
{
    double res = 0.0;
    yEnterCriticalSection(&_this_cs);
    try {
        if (_cacheExpiration <= YAPI::GetTickCount()) {
            if (this->_load_unsafe(YAPI::DefaultCacheValidity) != YAPI_SUCCESS) {
                {
                    yLeaveCriticalSection(&_this_cs);
                    return YWeighScale::ZEROTRACKING_INVALID;
                }
            }
        }
        res = _zeroTracking;
    } catch (std::exception) {
        yLeaveCriticalSection(&_this_cs);
        throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

string YWeighScale::get_command(void)
{
    string res;
    yEnterCriticalSection(&_this_cs);
    try {
        if (_cacheExpiration <= YAPI::GetTickCount()) {
            if (this->_load_unsafe(YAPI::DefaultCacheValidity) != YAPI_SUCCESS) {
                {
                    yLeaveCriticalSection(&_this_cs);
                    return YWeighScale::COMMAND_INVALID;
                }
            }
        }
        res = _command;
    } catch (std::exception) {
        yLeaveCriticalSection(&_this_cs);
        throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

int YWeighScale::set_command(const string& newval)
{
    string rest_val;
    int res;
    yEnterCriticalSection(&_this_cs);
    try {
        rest_val = newval;
        res = _setAttr("command", rest_val);
    } catch (std::exception) {
         yLeaveCriticalSection(&_this_cs);
         throw;
    }
    yLeaveCriticalSection(&_this_cs);
    return res;
}

/**
 * Retrieves a weighing scale sensor for a given identifier.
 * The identifier can be specified using several formats:
 * <ul>
 * <li>FunctionLogicalName</li>
 * <li>ModuleSerialNumber.FunctionIdentifier</li>
 * <li>ModuleSerialNumber.FunctionLogicalName</li>
 * <li>ModuleLogicalName.FunctionIdentifier</li>
 * <li>ModuleLogicalName.FunctionLogicalName</li>
 * </ul>
 *
 * This function does not require that the weighing scale sensor is online at the time
 * it is invoked. The returned object is nevertheless valid.
 * Use the method YWeighScale.isOnline() to test if the weighing scale sensor is
 * indeed online at a given time. In case of ambiguity when looking for
 * a weighing scale sensor by logical name, no error is notified: the first instance
 * found is returned. The search is performed first by hardware name,
 * then by logical name.
 *
 * If a call to this object's is_online() method returns FALSE although
 * you are certain that the matching device is plugged, make sure that you did
 * call registerHub() at application initialization time.
 *
 * @param func : a string that uniquely characterizes the weighing scale sensor
 *
 * @return a YWeighScale object allowing you to drive the weighing scale sensor.
 */
YWeighScale* YWeighScale::FindWeighScale(string func)
{
    YWeighScale* obj = NULL;
    int taken = 0;
    if (YAPI::_apiInitialized) {
        yEnterCriticalSection(&YAPI::_global_cs);
        taken = 1;
    }try {
        obj = (YWeighScale*) YFunction::_FindFromCache("WeighScale", func);
        if (obj == NULL) {
            obj = new YWeighScale(func);
            YFunction::_AddToCache("WeighScale", func, obj);
        }
    } catch (std::exception) {
        if (taken) yLeaveCriticalSection(&YAPI::_global_cs);
        throw;
    }
    if (taken) yLeaveCriticalSection(&YAPI::_global_cs);
    return obj;
}

/**
 * Registers the callback function that is invoked on every change of advertised value.
 * The callback is invoked only during the execution of ySleep or yHandleEvents.
 * This provides control over the time when the callback is triggered. For good responsiveness, remember to call
 * one of these two functions periodically. To unregister a callback, pass a NULL pointer as argument.
 *
 * @param callback : the callback function to call, or a NULL pointer. The callback function should take two
 *         arguments: the function object of which the value has changed, and the character string describing
 *         the new advertised value.
 * @noreturn
 */
int YWeighScale::registerValueCallback(YWeighScaleValueCallback callback)
{
    string val;
    if (callback != NULL) {
        YFunction::_UpdateValueCallbackList(this, true);
    } else {
        YFunction::_UpdateValueCallbackList(this, false);
    }
    _valueCallbackWeighScale = callback;
    // Immediately invoke value callback with current value
    if (callback != NULL && this->isOnline()) {
        val = _advertisedValue;
        if (!(val == "")) {
            this->_invokeValueCallback(val);
        }
    }
    return 0;
}

int YWeighScale::_invokeValueCallback(string value)
{
    if (_valueCallbackWeighScale != NULL) {
        _valueCallbackWeighScale(this, value);
    } else {
        YSensor::_invokeValueCallback(value);
    }
    return 0;
}

/**
 * Registers the callback function that is invoked on every periodic timed notification.
 * The callback is invoked only during the execution of ySleep or yHandleEvents.
 * This provides control over the time when the callback is triggered. For good responsiveness, remember to call
 * one of these two functions periodically. To unregister a callback, pass a NULL pointer as argument.
 *
 * @param callback : the callback function to call, or a NULL pointer. The callback function should take two
 *         arguments: the function object of which the value has changed, and an YMeasure object describing
 *         the new advertised value.
 * @noreturn
 */
int YWeighScale::registerTimedReportCallback(YWeighScaleTimedReportCallback callback)
{
    YSensor* sensor = NULL;
    sensor = this;
    if (callback != NULL) {
        YFunction::_UpdateTimedReportCallbackList(sensor, true);
    } else {
        YFunction::_UpdateTimedReportCallbackList(sensor, false);
    }
    _timedReportCallbackWeighScale = callback;
    return 0;
}

int YWeighScale::_invokeTimedReportCallback(YMeasure value)
{
    if (_timedReportCallbackWeighScale != NULL) {
        _timedReportCallbackWeighScale(this, value);
    } else {
        YSensor::_invokeTimedReportCallback(value);
    }
    return 0;
}

/**
 * Adapts the load cell signal bias (stored in the corresponding genericSensor)
 * so that the current signal corresponds to a zero weight.
 *
 * @return YAPI_SUCCESS if the call succeeds.
 *
 * On failure, throws an exception or returns a negative error code.
 */
int YWeighScale::tare(void)
{
    return this->set_command("T");
}

/**
 * Configures the load cell span parameters (stored in the corresponding genericSensor)
 * so that the current signal corresponds to the specified reference weight.
 *
 * @param currWeight : reference weight presently on the load cell.
 * @param maxWeight : maximum weight to be expectect on the load cell.
 *
 * @return YAPI_SUCCESS if the call succeeds.
 *
 * On failure, throws an exception or returns a negative error code.
 */
int YWeighScale::setupSpan(double currWeight,double maxWeight)
{
    return this->set_command(YapiWrapper::ysprintf("S%d:%d", (int) floor(1000*currWeight+0.5),(int) floor(1000*maxWeight+0.5)));
}

/**
 * Records a weight offset thermal compensation table, in order to automatically correct the
 * measured weight based on the compensation temperature.
 * The weight correction will be applied by linear interpolation between specified points.
 *
 * @param tempValues : array of floating point numbers, corresponding to all
 *         temperatures for which an offset correction is specified.
 * @param compValues : array of floating point numbers, corresponding to the offset correction
 *         to apply for each of the temperature included in the first
 *         argument, index by index.
 *
 * @return YAPI_SUCCESS if the call succeeds.
 *
 * On failure, throws an exception or returns a negative error code.
 */
int YWeighScale::set_offsetCompensationTable(vector<double> tempValues,vector<double> compValues)
{
    int siz = 0;
    int res = 0;
    int idx = 0;
    int found = 0;
    double prev = 0.0;
    double curr = 0.0;
    double currComp = 0.0;
    double idxTemp = 0.0;
    siz = (int)tempValues.size();
    if (!(siz != 1)) {
        _throw(YAPI_INVALID_ARGUMENT,"thermal compensation table must have at least two points");
        return YAPI_INVALID_ARGUMENT;
    }
    if (!(siz == (int)compValues.size())) {
        _throw(YAPI_INVALID_ARGUMENT,"table sizes mismatch");
        return YAPI_INVALID_ARGUMENT;
    }

    res = this->set_command("2Z");
    if (!(res==YAPI_SUCCESS)) {
        _throw(YAPI_IO_ERROR,"unable to reset thermal compensation table");
        return YAPI_IO_ERROR;
    }
    // add records in growing temperature value
    found = 1;
    prev = -999999.0;
    while (found > 0) {
        found = 0;
        curr = 99999999.0;
        currComp = -999999.0;
        idx = 0;
        while (idx < siz) {
            idxTemp = tempValues[idx];
            if ((idxTemp > prev) && (idxTemp < curr)) {
                curr = idxTemp;
                currComp = compValues[idx];
                found = 1;
            }
            idx = idx + 1;
        }
        if (found > 0) {
            res = this->set_command(YapiWrapper::ysprintf("2m%d:%d", (int) floor(1000*curr+0.5),(int) floor(1000*currComp+0.5)));
            if (!(res==YAPI_SUCCESS)) {
                _throw(YAPI_IO_ERROR,"unable to set thermal compensation table");
                return YAPI_IO_ERROR;
            }
            prev = curr;
        }
    }
    return YAPI_SUCCESS;
}

/**
 * Retrieves the weight offset thermal compensation table previously configured using the
 * set_offsetCompensationTable function.
 * The weight correction is applied by linear interpolation between specified points.
 *
 * @param tempValues : array of floating point numbers, that is filled by the function
 *         with all temperatures for which an offset correction is specified.
 * @param compValues : array of floating point numbers, that is filled by the function
 *         with the offset correction applied for each of the temperature
 *         included in the first argument, index by index.
 *
 * @return YAPI_SUCCESS if the call succeeds.
 *
 * On failure, throws an exception or returns a negative error code.
 */
int YWeighScale::loadOffsetCompensationTable(vector<double>& tempValues,vector<double>& compValues)
{
    string id;
    string bin_json;
    vector<string> paramlist;
    int siz = 0;
    int idx = 0;
    double temp = 0.0;
    double comp = 0.0;

    id = this->get_functionId();
    id = (id).substr( 11, (int)(id).length() - 11);
    bin_json = this->_download("extra.json?page=2");
    paramlist = this->_json_get_array(bin_json);
    // convert all values to float and append records
    siz = (((int)paramlist.size()) >> (1));
    tempValues.clear();
    compValues.clear();
    idx = 0;
    while (idx < siz) {
        temp = atof((paramlist[2*idx]).c_str())/1000.0;
        comp = atof((paramlist[2*idx+1]).c_str())/1000.0;
        tempValues.push_back(temp);
        compValues.push_back(comp);
        idx = idx + 1;
    }
    return YAPI_SUCCESS;
}

/**
 * Records a weight span thermal compensation table, in order to automatically correct the
 * measured weight based on the compensation temperature.
 * The weight correction will be applied by linear interpolation between specified points.
 *
 * @param tempValues : array of floating point numbers, corresponding to all
 *         temperatures for which a span correction is specified.
 * @param compValues : array of floating point numbers, corresponding to the span correction
 *         (in percents) to apply for each of the temperature included in the first
 *         argument, index by index.
 *
 * @return YAPI_SUCCESS if the call succeeds.
 *
 * On failure, throws an exception or returns a negative error code.
 */
int YWeighScale::set_spanCompensationTable(vector<double> tempValues,vector<double> compValues)
{
    int siz = 0;
    int res = 0;
    int idx = 0;
    int found = 0;
    double prev = 0.0;
    double curr = 0.0;
    double currComp = 0.0;
    double idxTemp = 0.0;
    siz = (int)tempValues.size();
    if (!(siz != 1)) {
        _throw(YAPI_INVALID_ARGUMENT,"thermal compensation table must have at least two points");
        return YAPI_INVALID_ARGUMENT;
    }
    if (!(siz == (int)compValues.size())) {
        _throw(YAPI_INVALID_ARGUMENT,"table sizes mismatch");
        return YAPI_INVALID_ARGUMENT;
    }

    res = this->set_command("3Z");
    if (!(res==YAPI_SUCCESS)) {
        _throw(YAPI_IO_ERROR,"unable to reset thermal compensation table");
        return YAPI_IO_ERROR;
    }
    // add records in growing temperature value
    found = 1;
    prev = -999999.0;
    while (found > 0) {
        found = 0;
        curr = 99999999.0;
        currComp = -999999.0;
        idx = 0;
        while (idx < siz) {
            idxTemp = tempValues[idx];
            if ((idxTemp > prev) && (idxTemp < curr)) {
                curr = idxTemp;
                currComp = compValues[idx];
                found = 1;
            }
            idx = idx + 1;
        }
        if (found > 0) {
            res = this->set_command(YapiWrapper::ysprintf("3m%d:%d", (int) floor(1000*curr+0.5),(int) floor(1000*currComp+0.5)));
            if (!(res==YAPI_SUCCESS)) {
                _throw(YAPI_IO_ERROR,"unable to set thermal compensation table");
                return YAPI_IO_ERROR;
            }
            prev = curr;
        }
    }
    return YAPI_SUCCESS;
}

/**
 * Retrieves the weight span thermal compensation table previously configured using the
 * set_spanCompensationTable function.
 * The weight correction is applied by linear interpolation between specified points.
 *
 * @param tempValues : array of floating point numbers, that is filled by the function
 *         with all temperatures for which an span correction is specified.
 * @param compValues : array of floating point numbers, that is filled by the function
 *         with the span correction applied for each of the temperature
 *         included in the first argument, index by index.
 *
 * @return YAPI_SUCCESS if the call succeeds.
 *
 * On failure, throws an exception or returns a negative error code.
 */
int YWeighScale::loadSpanCompensationTable(vector<double>& tempValues,vector<double>& compValues)
{
    string id;
    string bin_json;
    vector<string> paramlist;
    int siz = 0;
    int idx = 0;
    double temp = 0.0;
    double comp = 0.0;

    id = this->get_functionId();
    id = (id).substr( 11, (int)(id).length() - 11);
    bin_json = this->_download("extra.json?page=3");
    paramlist = this->_json_get_array(bin_json);
    // convert all values to float and append records
    siz = (((int)paramlist.size()) >> (1));
    tempValues.clear();
    compValues.clear();
    idx = 0;
    while (idx < siz) {
        temp = atof((paramlist[2*idx]).c_str())/1000.0;
        comp = atof((paramlist[2*idx+1]).c_str())/1000.0;
        tempValues.push_back(temp);
        compValues.push_back(comp);
        idx = idx + 1;
    }
    return YAPI_SUCCESS;
}

YWeighScale *YWeighScale::nextWeighScale(void)
{
    string  hwid;

    if(YISERR(_nextFunction(hwid)) || hwid=="") {
        return NULL;
    }
    return YWeighScale::FindWeighScale(hwid);
}

YWeighScale* YWeighScale::FirstWeighScale(void)
{
    vector<YFUN_DESCR>   v_fundescr;
    YDEV_DESCR             ydevice;
    string              serial, funcId, funcName, funcVal, errmsg;

    if(YISERR(YapiWrapper::getFunctionsByClass("WeighScale", 0, v_fundescr, sizeof(YFUN_DESCR), errmsg)) ||
       v_fundescr.size() == 0 ||
       YISERR(YapiWrapper::getFunctionInfo(v_fundescr[0], ydevice, serial, funcId, funcName, funcVal, errmsg))) {
        return NULL;
    }
    return YWeighScale::FindWeighScale(serial+"."+funcId);
}

//--- (end of YWeighScale implementation)

//--- (WeighScale functions)
//--- (end of WeighScale functions)
