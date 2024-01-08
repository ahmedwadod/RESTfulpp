#ifndef __RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>
namespace RESTfulpp {
class Uri {
public:
  Uri() {}
  Uri(std::string path_str, std::string query_str)
      : path(path_str), queryStr(query_str){};
  std::string path;
  std::string queryStr;
};
} // namespace RESTfulpp

#endif // !__RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__
