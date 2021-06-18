import {isEqual} from './utils.js'; 

class Sprite 
{
	constructor(spritesheetName, src){
		this.spritesheetName = spritesheetName ?? "";
		this.src = src ?? [0,0,0,0];
	}
}

class Entity 
{
	constructor(id)
	{
		if (!id){
			this.id = Math.random().toString(36).substring(7);
		} else {
			this.id = id;
		}

		this.isSelected = false;
		this.fields = {};
	}

	isDirty(displayedAttibutes)
	{
		//console.log("DEBUG: is dirty check", this.fields, displayedAttibutes)
		return !isEqual(this.fields, displayedAttibutes);
	}

	getSpriteSheetToDisplay()
	{
		if (!this.fields.graphic){
			return null;
		} 

		if (this.fields.graphic.spritesheet && this.fields.graphic.frame){
			return new Sprite(this.fields.graphic.spritesheet,  this.fields.graphic.frame);
		}

		const animations = this.fields.graphic.animations;
		if (!animations || animations.length == 0){
			return null;
		}
		const defaultAnimation = animations.find(e => e.type == 'default');
		if (defaultAnimation){
			return new Sprite(defaultAnimation.spritesheet, defaultAnimation.frames[0]);
		}

		const firstAnimation = animations[0];
		return new Sprite(firstAnimation.spritesheet, firstAnimation.frames[0]);
	}
} 
export {Entity, Sprite}