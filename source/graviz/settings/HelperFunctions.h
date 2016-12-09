/**
 * Project   Graviz
 *
 * @file     CHelperFunctions.h
 * @author   Dmytro Sadovyi
 * @date     05.12.2016
 */

#define DECLARE_ATTRIBUTE(classname, name, type) \
    public:\
        const type& get##name() const\
        { return m##name; }\
        C##classname& set##name(const type& param)\
        { m##name = param; return *this; }\
    private:\
        type m##name;

#define DECLARE_CLASS_HEAD(name) \
    class C##name \
    { \
    public: \
        static C##name& getInstance()\
        { \
            static C##name instance; \
            return instance; \
        } \
    private: \
        C##name(); \
        C##name(const C##name& other) = delete;

#define DECLARE_CLASS_END \
    };



