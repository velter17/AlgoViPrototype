/**
 * Project   Graviz
 *
 * @file     CHelperFunctions.h
 * @author   Dmytro Sadovyi
 * @date     05.12.2016
 */

#pragma once

#include <map>
#include <vector>
#include <string>

#define DECLARE_ATTRIBUTE(classname, name, type) \
    public:\
        const type& get##name() const\
        { return m##name; }\
        C##classname& set##name(const type& param)\
        { m##name = param; return *this; }\
    private:\
        type m##name;

#define DECLARE_ATTRIBUTE_VECTOR(classname, name, type) \
    public: \
        const type& get##name(int idx) \
        { return m##name.at(idx); } \
        C##classname& append##name(const type& param) \
        { m##name.push_back(param); return *this;} \
        C##classname& clear##name() \
        { m##name.clear(); return *this;} \
    private: \
        std::vector<type> m##name;

#define DECLARE_ATTRIBUTE_MAP(classname, name, type_key, type_value) \
    public: \
        const type_value& get##name(const type_key& key) \
        { return m##name[key]; } \
        C##classname& set##name(const type_key& key, const type_value& value) \
        { m##name[key] = value; return *this;} \
        bool is##name(const type_key& key) \
        { return m##name.find(key) != m##name.end(); } \
        C##classname& clear##name() \
        { m##name.clear(); return *this;} \
    private: \
        std::map<type_key, type_value> m##name;

#define DECLARE_CLASS_HEAD(name, config) \
    class C##name \
    { \
    public: \
        static C##name& getInstance()\
        { \
            static C##name instance(config); \
            return instance; \
        } \
    private: \
        C##name(const std::string& configPath); \
        C##name(const C##name& other) = delete;

#define DECLARE_CLASS_END() \
    };



