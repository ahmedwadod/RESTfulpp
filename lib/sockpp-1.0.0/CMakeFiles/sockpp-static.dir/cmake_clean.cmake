file(REMOVE_RECURSE
  "build/lib/libsockpp.a"
  "build/lib/libsockpp.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/sockpp-static.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
