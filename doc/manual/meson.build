manual_xml = files('manual.xml')

manual_command = [
  xsltproc,
  '-o', meson.current_build_dir()/'manual',
  manual_stylesheet,
  manual_xml]

custom_target('manual',
  build_by_default: true,
  output : 'manual',
  command : manual_command,
  install : false,
  install_dir : join_paths(get_option('datadir'), 'doc', meson.project_name()))

  meson.add_install_script('manual.sh')
