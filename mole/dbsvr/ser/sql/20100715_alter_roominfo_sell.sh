#!/bin/bash

#----modify--------
user="root"
password="ta0mee"
host="localhost"
tmp_file="table.sql"



#-------------
create_sell_sql() {
cat <<EOF >$tmp_file
	ALTER TABLE t_roominfo_item_sell_$1 add npcid INT UNSIGNED NOT NULL DEFAULT 0 AFTER userid;
	ALTER TABLE t_roominfo_item_sell_$1 drop primary key;
	ALTER TABLE t_roominfo_item_sell_$1 add primary key(npcid,userid,itemid);
EOF
}


db_index=0
end_index=10
table_index=0
	while [ $db_index -lt 10 ]; do
		echo $db_index
		dbx=`printf "%01d" $db_index`
			while [ $table_index -lt 10 ];do
				tabx=`printf "%d" $table_index` 
        		create_sell_sql $tabx
        		cat $tmp_file | mysql -u $user --password="$password" -h $host "ROOMINFO_$dbx"
				table_index=$(($table_index + 1))
			done
			table_index=0
			db_index=$(($db_index+1))
		done

