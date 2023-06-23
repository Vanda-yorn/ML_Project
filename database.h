
void createDataBase() {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
   char *sqlTable, *sqlTable1, *sqlTable2, *sqlTable3, *sqlTable4;
   const char **data;

   /* Open database */
rc = sqlite3_open("Automata.db", &db);


//SQL Create Table FA
sqlTable = "CREATE TABLE FA("\
            "id INT PRIMARY KEY NOT NULL,"\
            "fa_type CHAR(50) NOT NULL,"\
            "description CHAR(50) NOT NULL);";
rc = sqlite3_exec(db, sqlTable, NULL, 0, &zErrMsg);

//SQL Create Table Alphabet
sqlTable1 = "CREATE TABLE alphabet("\
            "fa_id INT NOT NULL,"\
            "symbol CHAR NOT NULL ,"\
            "PRIMARY KEY (fa_id,symbol) ,"\
            "FOREIGN KEY (fa_id) REFERENCES fa(id));";
rc = sqlite3_exec(db, sqlTable1, NULL, 0, &zErrMsg);

//SQL Create Table states
sqlTable2 = "CREATE TABLE states("\
            "fa_id  INT,"\
            "state INT NOT NULL,"\
            "start_state  INT NOT NULL, "\
            "final_state INT NOT NULL, "\
            "PRIMARY KEY (fa_id,state) , "\
            "FOREIGN KEY (fa_id) REFERENCES fa(id));";
rc = sqlite3_exec(db, sqlTable2, NULL, 0, &zErrMsg);
   if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error transition: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "successfully!\n");
    }


sqlTable3 = "CREATE TABLE transition("\
            "fa_id  INT NOT NULL,"\
            "state  INT NOT NULL ,"\
            "symbol CHAR NOT NULL ,"\
            "next_state INT ,"\
            "PRIMARY KEY (fa_id ,state ,symbol ,next_state),"\
            "FOREIGN KEY (fa_id) REFERENCES fa(id),"\
            "FOREIGN KEY (state) REFERENCES states(state),"\
            "FOREIGN KEY (symbol) REFERENCES alphabet(symbol),"\
            "FOREIGN KEY (next_state) REFERENCES states(state));";
rc = sqlite3_exec(db, sqlTable3,NULL,0,&zErrMsg);


sqlite3_close(db);
}
