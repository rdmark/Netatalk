manpages = ['afpd', 'cnid_dbd', 'cnid_metad', 'netatalk']
foreach man : manpages
custom_target(man + '.8',
  output :man + '.8',
  input : man + '.8.xml',
  command: manpages_command,
  install : true,
  install_dir : join_paths(get_option('mandir'), 'man8'))
endforeach
