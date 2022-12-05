project('netatalk', 'c',
  version : '3.1.3',
  license : 'GPLv2',
  default_options : ['warning_level=3', 'c_std=c2x', 'prefix=/usr/local'],
  meson_version : '>=0.61.0'
)

cc = meson.get_compiler('c')

add_global_arguments('-D_U_=__attribute__((unused))', language : 'c')

inc = include_directories('include' , 'etc/afpd', '.')

cdata = configuration_data()

build_html_mans = get_option('BUILD_HTML_MANS')
run_tests = not get_option('SKIP_TESTS')

fs = import('fs')

supported_oses = 'darwin'
supported_cpus = ['aarch64', 'x86_64']

if 'aarch64'.contains(host_machine.cpu_family())
  lib_crypto = cc.find_library('crypto', dirs : '/opt/homebrew/opt/libressl/lib', required: true)
  inc_crypto = include_directories('/opt/homebrew/opt/libressl/include')

  lib_db = cc.find_library('db', dirs : '/opt/homebrew/opt/berkeley-db/lib', required : true)
  inc_db = include_directories('/opt/homebrew/opt/berkeley-db/include')

elif 'x86_64'.contains(host_machine.cpu_family())
  lib_crypto = cc.find_library('crypto', dirs : '/usr/local/opt/libressl/lib', required: true)
  inc_crypto = include_directories('/usr/local/opt/libressl/include')

  lib_db = cc.find_library('db', dirs : '/usr/local/opt/berkeley-db/lib', required : true)
  inc_db = include_directories('/usr/local/opt/berkeley-db/include')

endif

kerberos = cc.find_library('krb5', required : true)
ldap = cc.find_library('ldap', required : true)
pam = cc.find_library('pam', required : true)
zeroconf = cc.find_library('system', required : true)

dbus = dependency('dbus-1', required : true)
dbusglib = dependency('dbus-glib-1', required : true)
glib = dependency('glib-2.0', required : true)
gssapi = dependency('appleframeworks', modules : 'GSS', required : true)
libevent = dependency('libevent', required : true)
libgcrypt = dependency('libgcrypt', required : true)
libiconv = dependency('iconv', method : 'system', required : true)
mysqlclient = dependency('mysqlclient', required : true)
threads = dependency('threads', required : true)

xsltproc = find_program('xsltproc', required : true)

cdata.set('DEFAULT_CNID_SCHEME', '"dbd"')
cdata.set('EA_MODULES', '"ad | sys"')
cdata.set('ICONV_CONST', '')
cdata.set('PATH_NETATALK_LOCK', '"/var/run/netatalk.pid"')
cdata.set('VERSION', '"3.1.13"')

configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : cdata)

subdir('libatalk')
subdir('bin')
subdir('config')
subdir('contrib')
subdir('distrib')
subdir('include')
subdir('etc')
subdir('doc')

if run_tests
	subdir('test')
endif
