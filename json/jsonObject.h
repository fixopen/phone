#pragma once
#pragma warning( disable :4786 4503 )

#include <vector>
#include <map>
#include <string>

#define JSON_NOFILE				"nofile"
#define JSON_SENDFILE			"sendfile"

enum ValueType {
    vtNull,
	vtBool,
	vtNumeric,
	vtString,
	vtObject,
	vtArray
};

struct jsonArray  ;
struct jsonObject ;

struct jsonValue {
	jsonValue() : type(vtNull), boolValue(false), numericValue(0.0), objectValue(0), arrayValue(0) {}
	jsonValue(double const v) : type(vtNumeric), boolValue(false), numericValue(v), objectValue(0), arrayValue(0) {}
	jsonValue(std::string const& v): type(vtString), boolValue(false), numericValue(0.0), stringValue(v),objectValue(0), arrayValue(0) {}
	jsonValue(jsonObject *jo):type(vtObject), boolValue(false), numericValue(0.0), objectValue(jo), arrayValue(0) {}
	jsonValue(jsonArray *ja): type(vtArray), boolValue(false), numericValue(0.0), objectValue(0), arrayValue(ja) {}
	jsonValue(bool v) :type(vtBool), boolValue(v), numericValue(0.0), objectValue(0), arrayValue(0) {}
    ValueType	type;
    bool		boolValue;
    double		numericValue;
    std::string stringValue ;
    jsonObject* objectValue ;
    jsonArray*  arrayValue  ;

};

struct jsonObject {
    std::map<std::string, jsonValue> properties;
	~jsonObject();
};

struct jsonArray {
    std::vector<jsonValue> items;
	~jsonArray();
};

//º¯Êý´¦Àí
jsonValue const parse(std::string const& str);
std::string const serialize(jsonObject const *obj);
typedef jsonObject const* (*Process)(jsonObject const& jo);
extern std::map<std::string ,std::map<std::string,Process> > handlers_; 
extern void registerProcessor();
extern std::string const jsonProcessor(std::string const& received);


