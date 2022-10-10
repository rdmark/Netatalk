uniconv_sources = [
  'iso8859_1_adapted.c',
  'uniconv.c']

executable('uniconv',
            uniconv_sources,
            include_directories : inc,
            link_with : libatalk,
            dependencies : mysqlclient,
            install : true)
