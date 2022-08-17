#include "Type.h"

using namespace std;

namespace sg
{
    //曾试图使用union，但如你所见union和非POD的配合是如此错综复杂，于是改用类型安全的C++17的std::variant
    /*
    union updata //使用union，可以实现同一类拥有不同类数据的指针，同时用智能指针管理对象资源，因为每个unique_ptr大小相同，所以不会产生额外空间。
    {
        unique_ptr<void> pnull;
        unique_ptr<bool> pbool;
        unique_ptr<double> pnumble;
        unique_ptr<string> pstring;
        unique_ptr<json> pjson;
        unique_ptr<vector<bool>> pvecbool;     // FIXME:原本打算用std::array,但其模板参数不仅要求类型，还要求大小，只好改用std::vector，然后不得不多写一大堆东西;
        unique_ptr<vector<double>> pvecnumber; //考虑使用类模板，但json_value被放在map<string,json_value>中，暂时找不到解决方法只好作罢。
        unique_ptr<vector<string>> pvecstring;
        unique_ptr<vector<json>> pvecjson;

        updata() : pnull(make_unique<void>()) {}
        ~updata(); // note: { // FIXME:不声明会报错} Reason:The C++ Standard says (9.5p4):Note : In general, one must use explicit destructor calls
        // and placement new operators to change the active member of a union.
        updata(bool b) : pbool(make_unique<bool>(b)) {}
        updata(double d) : pnumble(make_unique<double>(d)) {}
        updata(string s) : pstring(make_unique<string>(s)) {}
        updata(json j) : pjson(make_unique<json>(j)) {}
        updata(vector<bool> vb) : pvecbool(make_unique<vector<bool>>(vb)) {}
        updata(vector<double> vd) : pvecnumber(make_unique<vector<double>>(vd)) {}
        updata(vector<string> vs) : pvecstring(make_unique<vector<string>>(vs)) {}
        updata(vector<json> vj) : pvecjson(make_unique<vector<json>>(vj)) {}
    };
    */

    class json_value
    {
    private:
        variant<unique_ptr<void*>,
                unique_ptr<bool>,
                unique_ptr<double>,
                unique_ptr<string>,
                unique_ptr<json>,
                unique_ptr<vector<bool>>,
                unique_ptr<vector<double>>,
                unique_ptr<vector<string>>,
                unique_ptr<vector<json>>>
            v;
        sg_type type;

    public:
        json_value() : type(SG_NULL), v(make_unique<void*>()) {} // note:给类的union成员写构造函数以便初始化。
        ~json_value() = default;

        json_value(bool b) : type(SG_BOOL), v(make_unique<bool>(b)) {}
        json_value(double d) : type(SG_NUMBER), v(make_unique<double>(d)) {}
        json_value(string s) : type(SG_STRING), v(make_unique<string>(s)) {}
        json_value(json j) : type(SG_OBJECT), v(make_unique<json>(j)) {}
        json_value(vector<bool> vb) : type(SG_VECBOOL), v(make_unique<vector<bool>>(vb)) {}
        json_value(vector<double> vd) : type(SG_VECNUMBER), v(make_unique<vector<double>>(vd)) {}
        json_value(vector<string> vs) : type(SG_VECSTRING), v(make_unique<vector<string>>(vs)) {}
        json_value(vector<json> vo) : type(SG_VECOBJECT), v(make_unique<vector<json>>(vo)) {}

        auto gettype() { return type; }
        auto &getv() { return v; };
    };
} // namespace sg
