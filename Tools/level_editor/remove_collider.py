import bpy

class MYADDON_OT_remove_collider(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_remove_collider"
    bl_label = "コライダー 削除"
    bl_description = "['collider']カスタムプロパティを削除します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # カスタムプロパティを削除
        if "collider" in context.object:
            del context.object["collider"]
        if "collider_center" in context.object:
            del context.object["collider_center"]
        if "collider_size" in context.object:
            del context.object["collider_size"]

        return {'FINISHED'}