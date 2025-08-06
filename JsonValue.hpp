// GENERATED ON CLAUDE
#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cctype>

// Forward declarations
class JsonValue;
typedef std::map<std::string, JsonValue> JsonObject;
typedef std::vector<JsonValue> JsonArray;

class JsonValue
{
public:
    enum Type {
        TYPE_NULL,
        TYPE_BOOL,
        TYPE_NUMBER,
        TYPE_STRING,
        TYPE_OBJECT,
        TYPE_ARRAY
    };

private:
    Type _type;
    bool _bool_value;
    double _number_value;
    std::string _string_value;
    JsonObject _object_value;
    JsonArray _array_value;

public:
    // Constructors
    JsonValue() : _type(TYPE_NULL), _bool_value(false), _number_value(0.0) {}
    
    JsonValue(bool value) : _type(TYPE_BOOL), _bool_value(value), _number_value(0.0) {}
    
    JsonValue(double value) : _type(TYPE_NUMBER), _bool_value(false), _number_value(value) {}
    
    JsonValue(int value) : _type(TYPE_NUMBER), _bool_value(false), _number_value(static_cast<double>(value)) {}
    
    JsonValue(const std::string& value) : _type(TYPE_STRING), _bool_value(false), _number_value(0.0), _string_value(value) {}
    
    JsonValue(const char* value) : _type(TYPE_STRING), _bool_value(false), _number_value(0.0), _string_value(value) {}
    
    JsonValue(const JsonObject& value) : _type(TYPE_OBJECT), _bool_value(false), _number_value(0.0), _object_value(value) {}
    
    JsonValue(const JsonArray& value) : _type(TYPE_ARRAY), _bool_value(false), _number_value(0.0), _array_value(value) {}

    // Getters
    Type getType() const { return _type; }
    
    bool asBool() const { return _bool_value; }
    
    double asNumber() const { return _number_value; }
    
    int asInt() const { return static_cast<int>(_number_value); }
    
    const std::string& asString() const { return _string_value; }
    
    const JsonObject& asObject() const { return _object_value; }
    
    const JsonArray& asArray() const { return _array_value; }

    // Convenience methods
    bool isNull() const { return _type == TYPE_NULL; }
    bool isBool() const { return _type == TYPE_BOOL; }
    bool isNumber() const { return _type == TYPE_NUMBER; }
    bool isString() const { return _type == TYPE_STRING; }
    bool isObject() const { return _type == TYPE_OBJECT; }
    bool isArray() const { return _type == TYPE_ARRAY; }

    // Object access
    JsonValue& operator[](const std::string& key) {
        if (_type != TYPE_OBJECT) {
            _type = TYPE_OBJECT;
            _object_value.clear();
        }
        return _object_value[key];
    }

    const JsonValue& operator[](const std::string& key) const {
        static JsonValue null_value;
        if (_type != TYPE_OBJECT) return null_value;
        JsonObject::const_iterator it = _object_value.find(key);
        return (it != _object_value.end()) ? it->second : null_value;
    }

    // Array access
    JsonValue& operator[](size_t index) {
        if (_type != TYPE_ARRAY) {
            _type = TYPE_ARRAY;
            _array_value.clear();
        }
        if (index >= _array_value.size()) {
            _array_value.resize(index + 1);
        }
        return _array_value[index];
    }

    const JsonValue& operator[](size_t index) const {
        static JsonValue null_value;
        if (_type != TYPE_ARRAY || index >= _array_value.size()) return null_value;
        return _array_value[index];
    }

    size_t size() const {
        if (_type == TYPE_ARRAY) return _array_value.size();
        if (_type == TYPE_OBJECT) return _object_value.size();
        return 0;
    }
};

class JsonParser
{
private:
    std::string _json;
    size_t _pos;

    void skipWhitespace() {
        while (_pos < _json.length() && std::isspace(_json[_pos])) {
            _pos++;
        }
    }

    char peek() {
        skipWhitespace();
        return (_pos < _json.length()) ? _json[_pos] : '\0';
    }

    char next() {
        skipWhitespace();
        return (_pos < _json.length()) ? _json[_pos++] : '\0';
    }

    std::string parseString() {
        std::string result;
        if (next() != '"') return result; // Should be opening quote
        
        while (_pos < _json.length()) {
            char c = _json[_pos++];
            if (c == '"') break;
            if (c == '\\' && _pos < _json.length()) {
                char escaped = _json[_pos++];
                switch (escaped) {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    default: result += escaped; break;
                }
            } else {
                result += c;
            }
        }
        return result;
    }

    double parseNumber() {
        std::string number;
        while (_pos < _json.length()) {
            char c = _json[_pos];
            if (std::isdigit(c) || c == '-' || c == '+' || c == '.' || c == 'e' || c == 'E') {
                number += c;
                _pos++;
            } else {
                break;
            }
        }
        
        std::istringstream iss(number);
        double value;
        iss >> value;
        return value;
    }

    JsonValue parseValue() {
        char c = peek();
        
        if (c == '"') {
            return JsonValue(parseString());
        }
        else if (c == '{') {
            return JsonValue(parseObject());
        }
        else if (c == '[') {
            return JsonValue(parseArray());
        }
        else if (c == 't' || c == 'f') {
            return JsonValue(parseBool());
        }
        else if (c == 'n') {
            parseNull();
            return JsonValue(); // null value
        }
        else if (std::isdigit(c) || c == '-') {
            return JsonValue(parseNumber());
        }
        
        return JsonValue(); // Default to null
    }

    JsonObject parseObject() {
        JsonObject obj;
        next(); // consume '{'
        
        if (peek() == '}') {
            next(); // consume '}'
            return obj;
        }
        
        while (true) {
            // Parse key
            if (peek() != '"') break;
            std::string key = parseString();
            
            // Expect ':'
            if (next() != ':') break;
            
            // Parse value
            JsonValue value = parseValue();
            obj[key] = value;
            
            char delimiter = next();
            if (delimiter == '}') break;
            if (delimiter != ',') break;
        }
        
        return obj;
    }

    JsonArray parseArray() {
        JsonArray arr;
        next(); // consume '['
        
        if (peek() == ']') {
            next(); // consume ']'
            return arr;
        }
        
        while (true) {
            JsonValue value = parseValue();
            arr.push_back(value);
            
            char delimiter = next();
            if (delimiter == ']') break;
            if (delimiter != ',') break;
        }
        
        return arr;
    }

    bool parseBool() {
        if (_json.substr(_pos, 4) == "true") {
            _pos += 4;
            return true;
        } else if (_json.substr(_pos, 5) == "false") {
            _pos += 5;
            return false;
        }
        return false;
    }

    void parseNull() {
        if (_json.substr(_pos, 4) == "null") {
            _pos += 4;
        }
    }

public:
    JsonParser() : _pos(0) {}

    JsonValue parse(const std::string& json) {
        _json = json;
        _pos = 0;
        return parseValue();
    }
};

// Usage example for your bot
// class JsonHelper 
// {
// public:
//     static std::string extractAccessToken(const std::string& json_response) {
//         JsonParser parser;
//         JsonValue root = parser.parse(json_response);
        
//         if (root.isObject() && !root["access_token"].isNull()) {
//             return root["access_token"].asString();
//         }
//         return "";
//     }
    
//     static void printUserInfo(const std::string& json_response) {
//         JsonParser parser;
//         JsonValue root = parser.parse(json_response);
        
//         if (root.isArray() && root.size() > 0) {
//             JsonValue user = root[0];
//             if (user.isObject()) {
//                 std::cout << "User ID: " << user["id"].asInt() << std::endl;
//                 std::cout << "Login: " << user["login"].asString() << std::endl;
//                 std::cout << "Email: " << user["email"].asString() << std::endl;
                
//                 // Access nested objects
//                 if (!user["campus"].isNull() && user["campus"].isArray() && user["campus"].size() > 0) {
//                     JsonValue campus = user["campus"][0];
//                     std::cout << "Campus: " << campus["name"].asString() << std::endl;
//                 }
//             }
//         }
//     }
    
//     static void printProjectInfo(const std::string& json_response) {
//         JsonParser parser;
//         JsonValue root = parser.parse(json_response);
        
//         if (root.isArray()) {
//             for (size_t i = 0; i < root.size(); ++i) {
//                 JsonValue project = root[i];
//                 if (project.isObject()) {
//                     std::cout << "Project: " << project["project"]["name"].asString() << std::endl;
//                     std::cout << "Status: " << project["status"].asString() << std::endl;
//                     if (!project["final_mark"].isNull()) {
//                         std::cout << "Final Mark: " << project["final_mark"].asInt() << std::endl;
//                     }
//                     std::cout << "---" << std::endl;
//                 }
//             }
//         }
//     }
// };

//TO BE CONTINUED...