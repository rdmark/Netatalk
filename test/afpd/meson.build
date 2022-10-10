test_sources = [
  afpd_files,
  'test.c',
  'subtests.c',
  'afpfunc_helpers.c']

deps = [gssapi, kerberos, libgcrypt, mysqlclient]

selftest = executable(
  'afpd-self-test', test_sources,
  include_directories : inc,
  dependencies : deps,
  c_args : ['-DAPPLCNAME',
            '-DSERVERTEXT="/usr/local/var/netatalk/msg/"',
            '-D_PATH_AFPDUAMPATH="/usr/local/lib/netatalk/"',
            '-D_PATH_CONFDIR="/usr/local/etc/"',
            '-D_PATH_STATEDIR="/usr/local/var/netatalk/"'],
  link_with : libatalk)
test('afpd-self-test', selftest)
