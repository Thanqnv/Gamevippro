# Castle of knowledge
## Giới thiệu bản thân:
- Họ và tên: Nguyễn Văn Thắng
- MSSV: 23020707
- Lớp: INT2215 52

## Thử nghiệm game: 
### Giới thiệu game
*Castle of knowledge* là 1 game thuộc thể loại trí tuệ, phiêu lưu. Game thực hiện công việc đi tìm các rương kho báu mà trong đó chứa các từ khóa giúp người chơi có thể trả lời được câu hỏi ở hòm rương cuối cùng. Để tìm được rương thì người chơi phải tìm kiếm các chìa khóa để mở cửa để đi qua các khu vực của lâu đài và trong những hòm rương kho báu sẽ có những con quái vật bảo vệ kho báu. Với hòm rương cuối cùng bạn sẽ có 3 lần nhập mã khóa cho rương nếu thành công bạn sẽ giành chiến thắng, sau 3 lần đoán sai bạn sẽ thua.

### 1. Bắt đầu game
Nhấn vào nút **“PLAY”** để bắt đầu chơi game.

### 2. Các thành phần trong game
- **Player (Người chơi):** là đối tượng trò chơi chính của trò chơi. Nó sẽ được tạo bằng cách chia hình ảnh và tạo clips cho những hình ảnh được cắt ra ấy bằng cách chạy nó từ trái sang phải.
- **Background (Nền phía sau):** là hình ảnh những viên gạch được xây dựng để mục đích tạo thành không gian phía trong lâu đài.
- **Floor (Gạch tường):** là những viên gạch được xây dựng để tạo bề mặt cho Player (người chơi) có thể đứng được trên đó, ngoài ra nó giới hạn bên trong lâu đài.
- **Door (Cửa):** là những cánh cửa ngăn cách giữa các phần bên trong lâu đài.
- **Key (Chìa khóa):** là những chìa khóa mà Player (người chơi) đi tìm để mở cửa để di chuyển đến các nơi khác trong lâu đài.
- **Window (Cửa sổ):** nó là hình ảnh các cửa sổ trong lâu đài, nó giúp cho lâu đài được đẹp hơn.
- **Starfield (Bầu trời sao):** Nó là hình ảnh những ngôi sao bên ngoài bầu trời, giúp khung cảnh lâu đài được tự nhiên hơn.
- **Dust (Bụi):** là hạt bụi được hình thành khi Player (người chơi) nhảy liên tục 2 lần.
- **Tutorial (Hướng dẫn):** Là văn bản hướng dẫn cách chơi trong game.
- **Chest (Rương kho báu):** Sẽ có 2 loại rương kho báu, khi gặp những rương này thì Player (người chơi) sẽ mở ra được một từ khóa, từ khóa này là những gợi ý cho câu hỏi ở rương cuối cùng. Nếu bạn tìm đến được rương cuối cùng thì bạn sẽ phải nhập mật khẩu khóa cho cái rương cuối cùng này.
- **Keyword (Từ khóa):** Đây là những từ khóa gợi ý khi bạn mở được rương kho báu.
- **Monster (Quái vật):** Là những con canh giữ rương kho báu, nếu bạn chạm vào thì sẽ bị thua.

### 4. Cách chơi
Người chơi sẽ sử dụng các phím mũi tên phải, trái để di chuyển người chơi tương ứng với sang phải và sang trái. Di chuyển để đi tìm được những rương kho báu, và thu thập chìa khóa để có thể mở cửa và đi tìm được rương kho báu cuối cùng. Trả lời mã khóa nếu đúng thì bạn thắng, sau 3 lần đoán sai thì bạn sẽ thua.

### 5. Chiến thắng và thất bại
- **Chiến thắng:** Bạn chiến thắng khi trả lời đúng mã khóa.
- **Thất bại:** Bạn thất bại khi chạm vào quái vật hoặc sau 3 lần đoán mã khóa bị sai.

## 6. Source code game
- **res:** Chứa folder war chứa file âm thanh của toàn bộ trò chơi và folder gfx sẽ chữa toàn bộ hình ảnh được hiện thị trong game.
- **SDL2.dll, SDL2_image.dll, SDL2_mixer.dll, SDL2_ttf.dll:** Đây là thư viện SDL2 được sử dụng để thực hiện.
- **BigShouldersText-Black.ttf:** Là font chữ được sử dụng trong game.
- **Cnst.h:** Lưu các hằng số sử dụng trong game như kích cỡ window, kích thước các ô pixel, vận tốc tối đa của đối tượng người chơi, các hằng số sử dụng trong quá trình tạo clips.
- **Media.h, media.cpp:** 
    * Khai báo đối tượng Media, texture cho các đối tượng trong game, vector lưu trữ Clips các đối tượng được lập.
    * load hình ảnh và âm thanh sử dụng trong game.
    * Các hàm để để hiện thị, xóa các đối tượng lên màn hình.
    * Tạo các Clips Animation của các đối tượng.
- **Map.h, map.cpp:** 
    * Gọi các đối tượng Player, ActiveEntity. 
    * Đối tượng Map: nhận các tham số đầu vào là con trỏ chỉ vào đối tượng Media để lấy tài nguyên ảnh và âm thanh, con trỏ chỉ đến biến lưu trữ hiện tại của camera(x và y), và chỉ đến biến lưu trữ vị trí mục tiêu của camera để có thể cập nhật được camera. 
    * Map: tạo dữ liệu cho map để vẽ hình ảnh lên dựa trên map đó. 
    * Camerazones: khởi tạo camera và kích hoạt camera khi đến vùng khác. 
    * Maptexture: các vector chứa các texture hình ảnh. 
    * cameraShake: độ rung của camera lúc đầu là 0. 
    * getTile: kiểm tra tọa độ và trả về 1 tọa độ mà nó nằm trong bản đồ. 
    * setTile: kiểm tra nếu không vượt ngoài bản đồ thì set cho nó vị trí getTile. 
    * UpdateCamera: cập nhật camera dựa vào vị trí của player. 
    * SakeCamera: rung camera khi player bị rơi. 
    * Drawmap: vẽ map dựa trên dữ liệu đầu vào, có 4 loại là 0, 1, 2, 3 lần lượt là: không in, gạch nền chính để player đứng lên, ảnh phông phía sau, cửa sổ. 
    * drawParralax: vẽ các đối tượng cố định trên lên màn hình. 
    * checkCollision: kiểm tra va chạm giữa người chơi với các ô.
- **Entity.h, Entity.cpp:** 
    * Lớp Entity để quản lý toàn bộ các đối tượng chính, sự kiện chính trong game. 
    * Render: load đối tượng lên màn hình. 
    * Lớp AnimEntity: để quản lý các clips để cập nhật lên màn hình. 
    * SetAnim: cập nhật clips. 
    * UpdatePos: cập nhật vị trí. 
    * UpdateGravity: cập nhật vận tốc khi người chơi rơi, vận tốc rơi của người chơi càng ngày càng nhanh, theo quy luật trọng trường. 
    * UpdateCollisions: cập nhật các trạng thái va chạm giữa người chơi với đối tượng khác. 
    * Lớp player: render cập nhật hình ảnh lên màn hình; moveleft, moveright: di chuyển sang trái, phải; stun: người chơi bị tê liệt đứng lại khi bị rời cao xuống; dash: cập nhật clips khi đi sang trái, phải; jump: cập nhật clips khi nhảy; updatecollisions: làm cho người chơi không đi qua được gạch nền tạo thành các tường đi, cập nhật các trạng thái có chìa khóa, dùng chìa khóa, thêm chìa khóa. 
    * Lớp door: cập nhật chìa khóa để nếu có chìa thì sẽ cập nhật clips mở cửa. 
    * Lớp key: cập nhật trạng thái của chìa khóa, vẽ lên màn hình. 
    * Lớp chest: cập nhật trạng thái va chạm với chest thực hiện hành động mở rương, vẽ lên màn hình. 
    * Lớp quai: hiện thị lên màn hình, kiểm tra va chạm với người chơi.
- **Main.cpp:** 
    * Khởi tạo init, font, window, renderer. Hiển thị màn hình. Khởi tạo vector lưu các đối tượng chính của chương trình, cập nhật map. Vòng chạy menu. Vòng lặp chính kiểm tra nhấn “SPACE” nhảy, “DOUBLE SPACE” nhảy liên tục. Kiểm tra di chuyển trái, phải. 
    * Kiểm tra va chạm với các đối tượng. 
    * Khi gặp chest4(chest cuối cùng) in hình ảnh câu hỏi, kiểm tra sự kiện nhập vào màn hình, in lên màn hình, kiểm tra đúng, sai. Kiểm tra gặp quái. Phá hủy kết thúc game.

## Đánh giá
### Ưu điểm
- Di chuyển khá mượt mà, các hiệu ứng trong game được xử lý hiệu quả.
### Nhược điểm
- Trò chơi chưa thật sự thú vị, còn khá đơn điệu.
- Chưa thể thành một game hoàn chỉnh.
- Chưa có nhiều map.
- Các thông số in lên màn hình còn là một vị trí cụ thể được cài đặt sẵn.
### Phát triển
- Load thêm nhiều map.
- Đưa các thông số từng màn chơi vào file txt để dễ phát triển thêm các map mới.
- Load thêm nhiều monster hơn nữa, tạo độ khó cho game.
- Monster có thể nhận diện ra người chơi và đi lại gần.
- Thêm nhiều câu hỏi khó.

## Nguồn tham khảo
- [Tham khảo](https://github.com/Ambidextroid/SDL2-demo): Hiểu nguyên lý thực hiện của code.
- [Lazyfoo](https://lazyfoo.net/): hiểu nguyên lý làm việc của sự kiện chuột, load hình ảnh, âm thanh, chữ, cập nhật màn hình.
- Internet, youtube.

## Quá trình làm game
Trong quá trình làm game thì được học thêm một thể loại mới của lập trình, lần đầu tiên có thể code game. Phát triển thêm kiến thức, kỹ năng lập trình, chỉnh sửa hình ảnh. Ngoài ra, trong quá trình làm thì gặp rất nhiều khó khăn vì game là thể loại mới, kiến thức về git còn hạn chế nên có những lần thực hiện sai không thể đẩy code lên github được, phải hỏi chị menter rồi phải xóa toàn bộ rebo thực hiện lại từ đầu.
