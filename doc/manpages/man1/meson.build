manpages = ['ad', 'afpldaptest', 'afpstats', 'apple_dump', 'asip-status.pl', 'dbd', 'macusers', 'netatalk-config']
foreach man : manpages
custom_target(man + '.1',
  output :man + '.1',
  input : man + '.1.xml',
  command: manpages_command,
  install : true,
  install_dir : join_paths(get_option('mandir'), 'man1'))
endforeach
