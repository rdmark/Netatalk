run_command('docbook.sh', check: true)

man_stylesheet = files('man.xsl')
manual_stylesheet = files('html.xsl')

subdir('manpages')

if build_html_mans == true
  subdir('manual')
endif
