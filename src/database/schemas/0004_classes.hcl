table "classes" {
	schema = schema.main

	column "id" {
		type = integer
		null = false
		auto_increment = true
	}

	column "cycle_id" {
		type = integer
		null = false
	}

	column "created_at" {
		type = datetime
		null = false
	}

	column "notes" {
		type = text
	}

	primary_key {
		columns = [column.id]
	}

	foreign_key "classes_cycle_fk" {
		columns = [column.cycle_id]
		ref_columns = [table.cycles.column.id]

		on_update = CASCADE
		on_delete = CASCADE
	}

	index "classes_cycleid_idx" {
		columns = [column.cycle_id]
	}
}