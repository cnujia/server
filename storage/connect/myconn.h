/***********************************************************************/
/*  MYCONN.H     Olivier Bertrand    2007-2012                         */
/*                                                                     */
/*  This is the declaration file for the MySQL connection class and    */
/*  a few utility functions used to communicate with MySQL.            */
/*                                                                     */
/*  DO NOT define DLL_EXPORT in your application so these items are    */
/*  declared are imported from the Myconn DLL.                         */
/***********************************************************************/
#if defined(WIN32)
#include <winsock.h>
#else   // !WIN32
#include <sys/socket.h>
#endif  // !WIN32
#include <mysql.h>
#include <errmsg.h>

#if defined(WIN32) && defined(MYCONN_EXPORTS)
#if defined(DLL_EXPORT)
#define DllItem _declspec(dllexport)
#else   // !DLL_EXPORT
#define DllItem _declspec(dllimport)
#endif  // !DLL_EXPORT
#else   // !WIN32  ||        !MYCONN_EXPORTS
#define DllItem
#endif  // !WIN32

//#define TYPE_AM_MYSQL  (AMT)192
#define MYSQL_ENABLED  0x00000001
#define MYSQL_LOGON    0x00000002

typedef class MYSQLC *PMYC;

/***********************************************************************/
/*  Exported/Imported functions.                                       */
/***********************************************************************/
DllItem int  MYSQLtoPLG(char *);
DllItem int  MYSQLtoPLG(int);
DllItem enum enum_field_types PLGtoMYSQL(int, bool gdf = FALSE);
DllItem char *MyDateFmt(int);
DllItem char *MyDateFmt(char *);

/* -------------------------- MYCONN class --------------------------- */

/***********************************************************************/
/*  MYSQLC exported/imported class. A MySQL connection.                */
/***********************************************************************/
class DllItem MYSQLC {
  friend class TDBMYSQL;
  // Construction
 public:
  MYSQLC(void);

  // Implementation
  int    GetRows(void) {return m_Rows;}
  bool   Connected(void);

  // Methods
//  int     GetCurPos(void) {return (m_Res) ? N : 0;}
//  int     GetProgCur(void) {return N;}
  int     GetResultSize(PGLOBAL g, PSZ sql);
  int     Open(PGLOBAL g, PSZ host, PSZ db, PSZ user = "root",
                          PSZ pwd = "*", int pt = 0);
  ulong   GetThreadID(void);
  ulong   ServerVersion(void);
  const char *ServerInfo(void);
  int     KillQuery(ulong id);
  int     ExecSQL(PGLOBAL g, const char *query, int *w = NULL);
  int     PrepareSQL(PGLOBAL g, const char *query);
  int     ExecStmt(PGLOBAL g);
  int     BindParams(PGLOBAL g, MYSQL_BIND *bind);
  PQRYRES GetResult(PGLOBAL g, bool pdb = FALSE);
  int     Fetch(PGLOBAL g, int pos);
  char   *GetCharField(int i);
  int     GetFieldLength(int i);
  void    Rewind(void);
  void    FreeResult(void);
  void    Close(void);
	void    DiscardResults(void);

 protected:
  // Members
  MYSQL      *m_DB;         // The return from MySQL connection
  MYSQL_STMT *m_Stmt;       // Prepared statement handle
  MYSQL_RES  *m_Res;        // Points to MySQL Result
  MYSQL_ROW   m_Row;        // Point to current row
  int         m_Rows;       // The number of rows of the result
  int         N;
  int         m_Fields;     // The number of result fields
  }; // end of class MYSQLC

