netatalk_sources = [
  'afp_mdns.c',
  'afp_zeroconf.c',
  'netatalk.c']

executable('netatalk',
            netatalk_sources,
            include_directories : inc,
            link_with : libatalk,
            dependencies : [mysqlclient, zeroconf, libevent],
            c_args : ['-D_PATH_CONFDIR="/usr/local/etc/"',
                      '-D_PATH_STATEDIR="/usr/local/var/netatalk/"',
                      '-D_PATH_AFPD="/usr/local/sbin/afpd"',
                      '-D_PATH_CNID_METAD="/usr/local/sbin/cnid_metad"'],
            install : true,
            install_dir : get_option('sbindir'))
