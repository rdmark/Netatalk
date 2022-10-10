acl_sources = [
  'aclldap.h',
  'cache.c',
  'cache.h',
  'ldap.c',
  'ldap_config.c',
  'uuid.c']

  libacl = static_library('acl',
                           acl_sources,
                           include_directories : inc,
                           dependencies : ldap,
                           install : false)
