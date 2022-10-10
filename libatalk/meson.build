subdir('acl')
subdir('adouble')
subdir('bstring')
subdir('cnid')
subdir('dsi')
subdir('iniparser')
subdir('tdb')
subdir('util')
subdir('unicode')
subdir('vfs')

libatalk_sources = []

libatalk = both_libraries('atalk',
                           libatalk_sources,
                           include_directories : inc,
                           dependencies : mysqlclient,
                           link_whole : [libacl, libadouble, libbstring, libcnid, libdsi, libiniparser, libtdb, libunicode, libutil, libvfs],
                           soversion : 18,
                           install : true)
