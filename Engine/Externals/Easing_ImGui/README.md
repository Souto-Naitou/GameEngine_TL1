# 導入方法
## インストール
1. 下記のフォルダを任意の場所にコピーする
```
./easing
```
2. インクルードパスを設定する (Visual Studio 2022)
   1. `プロジェクト(P) > [プロジェクト名]のプロパティの(P) > VC++ ディレクトリ`
   2. `> 全般 > インクルード ディレクトリ` 欄の逆三角から `<編集...>` を押下
   ![image](https://github.com/user-attachments/assets/8ee420e5-1f31-4c1c-b870-a02cb262470c)
   3. 新規ウィンドウの右上にあるフォルダアイコンをクリック
   4. `$(ProjectDir)/[csvLoaderとeasingフォルダが入っているフォルダ]` を入力
   ![image](https://github.com/user-attachments/assets/94d61bdd-d187-4583-933c-60ff81f96a34)
   5. `OK` をクリックし閉じる

## 使用方法
### デバッグ用ウィンドウを表示する
![image](https://github.com/user-attachments/assets/f8a6271e-aef2-4461-b168-9cb28981b740)
1. EasingManagerのインスタンスを取得
```c++
#include <easing/EasingManager/EasingManager.h>
EasingManager* pEasingManager = EasingManager::GetInstance();
```
2. UIを描画する
```c++
pEasingManager->DrawUI();
```

### イージングを使用する
1. Easingクラスのインスタンスを作成
```c++
#include <easing/Easing.h>
/* 中略 /*

std::unique_ptr<Easing> pEasing;

// EaseInSine - 秒モード
pEasing = std::make_unique<Easing>("String ID", Easing::EaseType::EaseInSine, 5.0);

// EaseOutCubic - フレームモード
pEasing = std::make_unique<Easing>("String ID", Easing::EaseType::EaseOutCubic, 300u);

// EaseInOutQuad - マニュアルモード
pEasing = std::make_unique<Easing>("String ID", Easing::EaseType::EaseInOutQuad);
```
2. Easingを再生する
```c++
// イージング開始
pEasing->Start();
```
3. Easingを更新し、現在のtを得る
```c++
// 更新しながらtを取得する。Start関数を実行するまで t = 0
float t = pEasing->Update();
```
4. Easingをリセットする
```c++
// tを0にする。Resetするとフラグがリセットされ Start関数がまた実行できるようになる
pEasing->Reset();
```
5. EasingManagerからEasingを取得する
```c++
// 存在するインスタンス内を検索し インスタンスを返す。
// 見つからない場合 nullptrを返す;
Easing* pEasing = pEasingManager->GetEasing("String ID");
```
