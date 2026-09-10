table "todos" {
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

	column "title" {
		type = text
		null = false
	}

	column "description" {
		type = text
	}

	column "completed" {
		type = bool
		null = false
		default = false
	}

	primary_key {
		columns = [column.id]
	}

	foreign_key "todos_user_fk" {
		columns = [column.user_id]
		ref_columns = [table.users.column.id]

		on_update = CASCADE
		on_delete = CASCADE
	}

	index "todos_user_id_idx" {
		columns = [column.user_id]
	}
}