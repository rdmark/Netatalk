manpages = ['afp_signature.conf', 'afp_voluuid.conf', 'afp.conf', 'extmap.conf']
foreach man : manpages
custom_target(man + '.5',
  output :man + '.5',
  input : man + '.5.xml',
  command: manpages_command,
  install : true,
  install_dir : join_paths(get_option('mandir'), 'man5'))
endforeach
