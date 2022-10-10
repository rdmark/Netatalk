cnid_dbd_sources = [
  'comm.c',
  'db_param.c',
  'dbd_add.c',
  'dbd_dbcheck.c',
  'dbd_delete.c',
  'dbd_get.c',
  'dbd_getstamp.c',
  'dbd_lookup.c',
  'dbd_rebuild_add.c',
  'dbd_resolve.c',
  'dbd_search.c',
  'dbd_update.c',
  'dbif.c',
  'main.c',
  'pack.c']

executable('cnid_dbd',
            cnid_dbd_sources,
            include_directories : [inc, inc_db],
            link_with : libatalk,
            dependencies : [mysqlclient, lib_db],
            install : true,
            install_dir : get_option('sbindir'))

cnid_metad_sources =[
  'cnid_metad.c',
  'usockfd.c',
  'db_param.c']

executable('cnid_metad',
            cnid_metad_sources,
            include_directories : [inc, inc_db],
            link_with : libatalk,
            dependencies : mysqlclient,
            c_args : '-D_PATH_CNID_DBD="/usr/local/sbin/cnid_dbd"',
            install : true,
            install_dir : get_option('sbindir'))

dbd_sources = [
  'cmd_dbd.c',
  'cmd_dbd_scanvol.c',
  'dbd_add.c',
  'dbd_delete.c',
  'dbd_getstamp.c',
  'dbd_lookup.c',
  'dbd_rebuild_add.c',
  'dbd_resolve.c',
  'dbd_update.c',
  'dbif.c',
  'pack.c']

  executable('dbd',
              dbd_sources,
              include_directories : [inc, inc_db],
              link_with : libatalk,
              dependencies : [mysqlclient, lib_db],
              install : true)
