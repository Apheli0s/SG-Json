#ifndef SG_Json_H
#define SG_Json_H

#include "Type.h"
#include "iostream"
#include "JsonValue.h"

namespace sg
{
    class json
    {
    public:
        json(/* args */);
        ~json();
        json(const string &json_text, int i = 0);

        size_t size() { return members.size(); }
        template <class T>
        void insert(const string &key, T param) { members[key] = make_unique<json_value>(param); }

        inline json_value &operator[](string s) { return members[s]; }

    private:
        map<string, json_value> members; //以json_value封装数据，并在其内部使用c++17 std::variant保存不同类型数据的unique_ptr,避免需要多个map管理数据。

        void parsevalue(const string &key, const string &json_text, int i);
        void parsenull(const string &key, const string &json_text, int i, const char *context);
        void parsebool(const string &key, const string &json_text, int i, const char *context);
        void parsenum(const string &key, const string &json_text, int i);
        void parsestring(const string &key, const string &json_text, int i);
        void parsevector(const string &key, const string &json_text, int i);
        void parseobject(const string &key, const string &json_text, int i);
    };

} // namespace sg

#endif
