table "students" {
	schema = schema.main

	column "id" {
		type = integer
		null = false
		auto_increment = true
	}

	column "user_id" {
		type = integer
		null = false
	}

	column "name" {
		type = text
		null = false
	}

	column "fee" {
		type = integer
		null = false
	}

	primary_key {
		columns = [column.id]
	}

	foreign_key "students_user_fk" {
		columns = [column.user_id]
		ref_columns = [table.users.column.id]

		on_update = CASCADE
		on_delete = CASCADE
	}

	index "students_user_id_idx" {
		columns = [column.user_id]
	}
}