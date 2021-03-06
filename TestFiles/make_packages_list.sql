SELECT ''''||OBJECT_NAME||''','  FROM all_objects WHERE owner = 'SYS' AND object_type = 'PACKAGE';

SELECT 'lng.0.grp.4.kwd.'||(rownum+22)||'='||method FROM (
SELECT DISTINCT INITCAP(package_name) method
  FROM all_arguments WHERE owner = 'SYS'
   AND package_name IN
   (
    'DBMS_APPLICATION_INFO',
    'DBMS_AQ',
    'DBMS_AQADM',
    'DBMS_DDL',
    'DBMS_DEBUG',
    'DBMS_DESCRIBE',
    'DBMS_JOB',
    'DBMS_LOB',
    'DBMS_OUTPUT',
    'DBMS_RANDOM',
    'DBMS_REFRESH',
    'DBMS_RESOURCE_MANAGER',
    'DBMS_RESOURCE_MANAGER_PRIVS',
    'DBMS_RMGR_GROUP_EXPORT',
    'DBMS_RMGR_PACT_EXPORT',
    'DBMS_RMGR_PLAN_EXPORT',
    'DBMS_ROWID',
    'DBMS_RULE',
    'DBMS_SESSION',
    'DBMS_SNAPSHOT',
    'DBMS_SNAPSHOT_UTL',
    'DBMS_SPACE',
    'DBMS_SQL',
    'DBMS_STATS',
    'DBMS_SUMMARY',
    'DBMS_TRACE',
    'DBMS_TRANSACTION',
    'DBMS_UTILITY',
    'HTF',
    'HTP',
    'UTL_FILE',
    'UTL_HTTP',
    'UTL_INADDR',
    'UTL_INADDR',
    'UTL_RAW',
    'UTL_REF',
    'UTL_SMTP',
    'UTL_TCP'
   )
UNION
SELECT DISTINCT INITCAP(package_name)||'.'||INITCAP(object_name)
  FROM all_arguments WHERE owner = 'SYS'
   AND package_name IN
   (
    'DBMS_APPLICATION_INFO',
    'DBMS_AQ',
    'DBMS_AQADM',
    'DBMS_DDL',
    'DBMS_DEBUG',
    'DBMS_DESCRIBE',
    'DBMS_JOB',
    'DBMS_LOB',
    'DBMS_OUTPUT',
    'DBMS_RANDOM',
    'DBMS_REFRESH',
    'DBMS_RESOURCE_MANAGER',
    'DBMS_RESOURCE_MANAGER_PRIVS',
    'DBMS_RMGR_GROUP_EXPORT',
    'DBMS_RMGR_PACT_EXPORT',
    'DBMS_RMGR_PLAN_EXPORT',
    'DBMS_ROWID',
    'DBMS_RULE',
    'DBMS_SESSION',
    'DBMS_SNAPSHOT',
    'DBMS_SNAPSHOT_UTL',
    'DBMS_SPACE',
    'DBMS_SQL',
    'DBMS_STATS',
    'DBMS_SUMMARY',
    'DBMS_TRACE',
    'DBMS_TRANSACTION',
    'DBMS_UTILITY',
    'HTF',
    'HTP',
    'UTL_FILE',
    'UTL_HTTP',
    'UTL_INADDR',
    'UTL_INADDR',
    'UTL_RAW',
    'UTL_REF',
    'UTL_SMTP',
    'UTL_TCP'
   )
)