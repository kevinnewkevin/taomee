#!/bin/bash
user="root"
password="ta0mee"
host="localhost"
tmp_file="table.sql"
create_user_treasure_show_table_sql() {
cat <<EOF >$tmp_file
CREATE TABLE IF NOT EXISTS t_user_treasure_show_$1(
	userid                   INT UNSIGNED NOT NULL DEFAULT 0,
	treasure_id              INT UNSIGNED NOT NULL DEFAULT 0,
	pos 		             INT UNSIGNED NOT NULL DEFAULT 0,
	status					 INT UNSIGNED NOT NULL DEFAULT 0,
	primary key (userid, pos)
	)ENGINE=innodb,CHARSET=utf8;
EOF
}

db_index=0
end_index=100

while [ $db_index -lt $end_index ] ; do
	dbx=`printf "%02d" $db_index`
	echo $dbx
	table_index=0
	while [ $table_index -lt 10 ] ; do
		tbx=`printf "%d" $table_index`
		create_user_treasure_show_table_sql $tbx 
		cat $tmp_file | mysql -u $user --password="$password" -h $host "USER_$dbx" 
		let "table_index+=1"
	done
	let "db_index+=1"
done

