import {EntityProperties} from './entity_properties.js'; 
import {Entity} from '../entity.js'
import { isEqual } from '../utils.js';

const CursorModeEnum = Object.freeze({"SELECT":1, "DRAW":2, "ERASE": 3});
let currentCursorMode = 0;

const entityMemory = {};

class Palette
{
    static refresh()
    {	
        const elem = document.getElementById("palette-entries");
        elem.innerHTML = "";//reset
    
        Object.keys(entityMemory).forEach(key=>{
            const entry = document.createElement("canvas"); //It's not optimal to make this all indivual canvas elements, but it works for now..TODO: use CSS?
            entry.setAttribute("data-entity-id", key); 
            entry.addEventListener("click", e=>{
                const entityId = e.target.getAttribute("data-entity-id");
                Palette.selectEntity(entityId);
                e.stopPropagation();
            });
    
            elem.appendChild(entry);
            entry.width = 50;
            entry.height = 50;
    
            const sprite = entityMemory[key].getSpriteSheetToDisplay();
            if (sprite){
                const img = new Image();
                img.onload = (function(){
                    const ctx = entry.getContext('2d');
                    const src = sprite.src;
                    const scale = entry.offsetHeight / this.height;
                    
                    ctx.drawImage(img,src[0],src[1], src[2], src[3], 0, 0, this.width * scale, this.height * scale);
                    //TODO: Make fit better, if is h cut off, move to left etc.
                })
                img.src = 'resources/spritesheets/' + sprite.spritesheetName +'.png';
            }
        });
    
        const selectedEntityId = Object.keys(entityMemory).find(e => entityMemory[e].isSelected);
        const onPalette = document.querySelector("#palette-entries [data-entity-id='" + selectedEntityId + "']");
        if (onPalette){
            onPalette.classList.add("selected-entity")
        }
    }

    static addNewEntity()
    {
        const entity = new Entity();
        Palette.addEntity(entity);
        EntityProperties.display(entity);
        //refresh();
    }   

    static addEntity(entity)
    {
        entityMemory[entity.id] = entity;
    }

    static getEntity(id)
    {
        return entityMemory[id];
    }

    static deleteEntity()
    {
        const id = Object.keys(entityMemory).find(e => entityMemory[e].isSelected);
        const name = entityMemory[id].fields.name ? " (" + entityMemory[id].fields.name + ")": "";

        if (!confirm("Delete entity " + id + name  +" ?" )){
            return;
        }
        
        delete(entityMemory[id]);
        Palette.refresh();
        //LevelField.refresh();
    }

    static selectEntity(entityId)
    {
        EntityProperties.display(entityMemory[entityId], entityMemory);
        if (currentCursorMode == CursorModeEnum.ERASE){
            Palette.flipEraser();
        }
        Palette.setCursorMode(CursorModeEnum.DRAW);
    }

    static deselectEntity()
    {
        if (!EntityProperties.confirmContinue()){
            return;
        }

        const elem = document.querySelector("#palette-entries .selected-entity");
        if (elem){
            const id = elem.getAttribute("data-entity-id");
            entityMemory[id].isSelected = false;
            elem.classList.remove("selected-entity");
        }
        EntityProperties.disable();
        Palette.setCursorMode(CursorModeEnum.SELECT);
    }

    static getSelectedEntity()
    {
        return entityMemory[Object.keys(entityMemory).find(e => entityMemory[e].isSelected)];
    }

    /** Checks if an entity like this exists on the palette, returns ID if it does, false if not */
    static entityExists(entityProperties)
    {
        // const found = Object.keys(entityMemory).filter(key=>{
        //     return isEqual(entityMemory[key].fields, entityProperties);
        // });

        const keys = Object.keys(entityMemory);
        for (let i = 0; i < keys.length ;i++){
            if (isEqual(entityMemory[keys[i]].fields, entityProperties)){
                return entityMemory[keys[i]];
            }
        }
        return false;
    }

    static setCursorMode(mode)
    {
        switch(mode){
            case CursorModeEnum.SELECT:
                document.getElementById('level-editor-canvas').style.cursor = "pointer";
                break;
            case CursorModeEnum.DRAW:
                document.getElementById('level-editor-canvas').style.cursor = "cell";
                break;
            case CursorModeEnum.ERASE:
                document.getElementById('level-editor-canvas').style.cursor = "crosshair";
                break;
            default:
                console.error("setCursorMode: invalid mode", mode);
                return;
        }
        currentCursorMode = mode;
    }

    static getCursorMode()
    {
        return currentCursorMode;
    }

    static flipEraser(buttonElem)
    {
        if (!buttonElem){
            buttonElem = document.getElementById("tools-eraser");
        }

        if (currentCursorMode == CursorModeEnum.ERASE){
			Palette.setCursorMode(CursorModeEnum.SELECT);
			buttonElem.style.fontWeight = "normal";
		} else {
			Palette.deselectEntity();
			Palette.setCursorMode(CursorModeEnum.ERASE);
			buttonElem.style.fontWeight = "bold";
		}
    }
}

export {Palette, CursorModeEnum}
