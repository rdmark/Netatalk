subdir('charsets')

unicode_sources = [
  'charcnv.c',
  'iconv.c',
  'precompose.h',
  'utf8.c',
  'utf16_case.c',
  'utf16_casetable.h',
  'util_unistr.c']

libunicode = static_library('unicode',
                             unicode_sources,
                             include_directories : inc,
                             dependencies : libiconv,
                             link_with : libcharsets,
                             install : false)
