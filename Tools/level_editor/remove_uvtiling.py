import bpy

class MYADDON_OT_remove_uvtiling(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_remove_uvtiling"
    bl_label = "UV Tiling 削除"
    bl_description = "['uv_tiling']カスタムプロパティを削除します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # カスタムプロパティを削除
        if "uv_tiling" in context.object:
            del context.object["uv_tiling"]

        return {'FINISHED'}