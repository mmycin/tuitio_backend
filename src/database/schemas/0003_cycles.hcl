table "cycles" {
	schema = schema.main

	column "id" {
		type = integer
		null = false
		auto_increment = true
	}

	column "student_id" {
		type = integer
		null = false
	}

	column "started_at" {
		type = datetime
		null = false
	}

	column "class_count" {
		type = integer
		null = false
		default = 0
	}

	column "is_paid" {
		type = bool
		default = false
	}

	primary_key {
		columns = [column.id]
	}

	foreign_key "cycles_student_fk" {
		columns = [column.student_id]
		ref_columns = [table.students.column.id]

		on_update = CASCADE
		on_delete = CASCADE
	}

	index "cycles_student_id_idx" {
		columns = [column.student_id]
	}
}