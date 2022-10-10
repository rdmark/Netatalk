install_data(sources : 'com.netatalk.daemon.plist',
             install_dir : '/Library/LaunchDaemons')

install_data('netatalkd',
              install_dir : get_option('bindir'))

meson.add_install_script('launchctl.sh')
