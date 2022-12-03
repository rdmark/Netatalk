conffile = fs.exists('/usr/local/etc/afp.conf')

if conffile
else
  install_data('afp.conf',
                install_dir : get_option('sysconfdir'))
endif

install_data('extmap.conf',
              install_dir : get_option('sysconfdir'))

if 'aarch64'.contains(host_machine.cpu_family())
  install_data('netatalk-dbus.conf',
                install_dir : '/opt/homebrew/etc/dbus-1/system.d')
elif 'x86_64'.contains(host_machine.cpu_family())
  install_data('netatalk-dbus.conf',
                install_dir : '/usr/local/etc/dbus-1/system.d')
endif

localstate_dir = join_paths(get_option('prefix'), 'var', meson.project_name())

install_data('README',
              install_dir : localstate_dir)

install_data('README',
              install_dir : join_paths(localstate_dir, 'CNID'))
