import {LevelField} from './level_editor/modules/level_field.js'; 
import {EntityProperties} from './level_editor/modules/entity_properties.js'; 
import {Palette} from './level_editor/modules/palette.js'; 

const ViewEnum = Object.freeze({"NONE":0, "EDITOR":1, "GAME":2});
 
function switchView(view)
{
	const elems = Array.from(document.getElementsByClassName("view"));
	elems.forEach(e=>{
		e.style.display = "none";
	})

	switch(view)
	{
		case ViewEnum.EDITOR:
			document.getElementById("view-editor").style.display = "block";
			break;
		case ViewEnum.GAME:
			document.getElementById("view-game").style.display = "block";
			break;
		// case ViewEnum.NONE:
		// 	return;
		default:
			break;
	}
}

function startGame()
{
	switchView(ViewEnum.GAME)
}

window.onload = function(e){ 	
	switchView(ViewEnum.EDITOR);
	document.getElementById("play").addEventListener("click", startGame)
	
	document.getElementById("grid-control").addEventListener("click", LevelField.flipGrid);
	document.getElementById("export-level").addEventListener("click", LevelField.export);
	document.getElementById("clear-level").addEventListener("click", LevelField.clear);
	document.getElementById("import-level").addEventListener("click", LevelField.import.bind(this, Palette));

	document.getElementById("palette-new").addEventListener("click", Palette.addNewEntity);
	document.getElementById("palette-delete").addEventListener("click",Palette.deleteEntity);
	document.getElementById("tools-eraser").addEventListener("click", e=>{
		Palette.flipEraser(e.target);
		e.stopPropagation();
	});

	document.getElementById("palette").addEventListener("click", Palette.deselectEntity);

	document.getElementById("save-entity-properties").addEventListener("click", EntityProperties.save);
	document.getElementById("validate-entity-properties").addEventListener("click",EntityProperties.validate);
	document.getElementById("add-new-entity-property").addEventListener("click", EntityProperties.addNew.bind(this, "", ""));
	
	document.getElementById("hide-graphic-data").addEventListener("click", e =>{
		EntityProperties.hideGraphicInfo(e.target.checked);
	});

	LevelField.initCanvas();

	LevelField.loadLevel();

	//Palette.refresh();
	EntityProperties.disable();
	//setCursorMode(CursorModeEnum.SELECT);
}