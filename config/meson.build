conffile = fs.exists('/usr/local/etc/afp.conf')

if conffile
else
  install_data('afp.conf',
                install_dir : get_option('sysconfdir'))
endif

install_data('extmap.conf',
              install_dir : get_option('sysconfdir'))

install_data(sources : 'netatalk-dbus.conf',
             install_dir : '/opt/homebrew/etc/dbus-1/system.d')

install_data('README',
              install_dir : join_paths(get_option('localstatedir'), meson.project_name()))

install_data('README',
              install_dir : join_paths(get_option('localstatedir'), meson.project_name(), 'CNID'))
