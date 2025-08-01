import bpy
import bpy_extras
import json

from . import connect_engine as ec

class MYADDON_OT_export_scene(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    bl_idname = "myaddon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    bl_description = "シーン情報をExportします"
    filename_ext = ".json"

    def write_and_print(self, f, str):
        print(str)
        f.write(str + '\n')

    def parse_scene_recursive(self, file, object, level):
        """シーン解析用再帰関数"""

        # 深さ分インデントする (タブを挿入)
        indent = ''
        for i in range(level):
            indent += '\t'

        # オブジェクト名書き込み
        self.write_and_print(file, indent + object.type)
        trans, rot, scale = object.matrix_local.decompose()
        # 回転をQuaternionからEulerに変換
        rot = rot.to_euler()
        # 出力
        self.write_and_print(file, indent + "T %f, %f, %f" % (trans.x, trans.y, trans.z))
        self.write_and_print(file, indent + "R %f, %f, %f" % (rot.x, rot.y, rot.z))
        self.write_and_print(file, indent + "S %f, %f, %f" % (scale.x, scale.y, scale.z))
        # カスタムプロパティ 'file_name'
        if "file_name" in object:
            self.write_and_print(file, indent + "N %s" % object['file_name'])
        # カスタムプロパティ 'collision'
        if "collider" in object:
            self.write_and_print(file, indent + "C %s" % object['collider'])
            temp_str = indent + "CC %f %f %f"
            temp_str %= (object["collider_center"][0], object["collider_center"][1], object["collider_center"][2])
            self.write_and_print(file, temp_str)
            temp_str = indent + "CS %f %f %f"
            temp_str %= (object["collider_size"][0], object["collider_size"][1], object["collider_size"][2])
            self.write_and_print(file, temp_str)

        self.write_and_print(file, indent + 'END')
        self.write_and_print(file, '')


        
        # 子ノードへ進む (深さが1増える)
        for child in object.children:
            # 子ノードを再帰的に走査
            self.parse_scene_recursive(file, child, level + 1)

    def parse_scene_recursive_json(self, data_parent, object, level):
        # シーンのオブジェクト1個分のJSONオブジェクトを生成
        json_object = dict()
        # オブジェクト種類
        json_object["type"] = object.type
        # オブジェクト名
        json_object["name"] = object.name
        
        # Todo: その他情報をパック
        trans, rot, scale = object.matrix_local.decompose()
        rot = rot.to_euler()

        transform = dict()
        transform["translation"] = (trans.x, trans.y, trans.z)
        transform["rotation"] = (rot.x, rot.y, rot.z)
        transform["scales"] = (scale.x, scale.y, scale.z)
        json_object["transform"] = transform

        # カスタムプロパティ 'file_name'
        if "file_name" in object:
            json_object["file_name"] = object["file_name"]
        # カスタムプロパティ 'collider'
        if "collider" in object:
            collider = dict()
            collider["type"] = object["collider"]
            collider["center"] = object["collider_center"].to_list()
            collider["size"] = object["collider_size"].to_list()
            json_object["collider"] = collider
        # カスタムプロパティ 'uv_tiling'
        if "uv_tiling" in object:
            json_object["uv_tiling"] = object["uv_tiling"].to_list()

        # 1個分のJSONオブジェクトを親オブジェクトに登録
        data_parent.append(json_object)

        # Todo: 直接子供リストを走査
        if len(object.children) > 0:
            # 子ノードリストを作成
            json_object["children"] = list()
            
            # 子ノードへ進む (深さが１増える)
            for child in object.children:
                self.parse_scene_recursive_json(json_object["children"], child, level + 1)

            


    def export(self):
        """ファイルに出力"""

        print("シーン情報出力開始... %r" % self.filepath)

        # ファイルをテキスト形式で書き込み用にオープン
        # スコープを抜けると自動的にクローズされる <- いいね！
        with open(self.filepath, "wt") as f:
            # ファイルに文字列を書き込む
            self.write_and_print(f, "SCENE")
            self.write_and_print(f, '')
            for object in bpy.context.scene.objects:
                # 親オブジェクトがあるものはスキップ (代わりに親から呼び出すため)
                if object.parent:
                    continue

                # シーン直下のオブジェクトをルートノードとし、再帰関数で走査
                self.parse_scene_recursive(f, object, 0)

    def export_json(self):
        """JSON形式でファイルに出力"""

        # 保存する情報をまとめるdictionary
        json_object_root = dict()

        # ノード名
        json_object_root["name"] = "scene"
        # オブジェクトのリストを作成
        json_object_root["objects"] = list()

        # Todo: シーン内の全オブジェクトを走査してパック
        for object in bpy.context.scene.objects:
            # 親オブジェクトがあるものはスキップ (代わりに親から呼び出すため)
            if (object.parent):
                continue
            # シーン直下のオブジェクトをルートノードとし、再帰関数で走査
            self.parse_scene_recursive_json(json_object_root["objects"], object, 0)

        # オブジェクトをJSON文字列にエンコード
        json_text = json.dumps(
            json_object_root, 
            ensure_ascii=False, 
            cls=json.JSONEncoder,
            indent=4)
        
        # コンソールに表示
        print(json_text)

        # ファイルをテキスト形式で書き込み用にオープン
        # スコープを抜けると自動的にクローズされる
        with open(self.filepath, "wt", encoding="utf-8") as f:
            # ファイルに文字列を書き込む
            f.write(json_text)
        
        # エンジンに通知
        if ec.g_is_connected:
            ec.g_engine.send_order("RequestReloadScene")

    def execute(self, context):
        print("シーン情報をExportします。")

        # ファイルに出力
        self.export_json()

        print("シーン情報をExportしました。")
        self.report({'INFO'}, "シーン情報をExportしました。")

        # オペレータの命令終了を通知
        return {'FINISHED'}

