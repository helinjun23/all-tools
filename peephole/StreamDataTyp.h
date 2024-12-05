#ifndef _STREAM_DATA_TYP_H_
#define _STREAM_DATA_TYP_H_

typedef enum MediaDecTyp {
	MediaDecTypSoft = 0,
	MediaDecTypHw,
	MediaDecTypEnd
}MediaDecTyp;

typedef enum MediaDstFmt {
	MediaDstFmtYuv = 0,
	MediaDstFmtRGB,
	MediaDstFmtBGR,
	MediaDstFmtRGBA,
	MediaDstFmtBGRA,
	MediaDstFmtEnd
}MediaDstFmt;

typedef enum MediaShowFmt {
	MediaShowFmtOpencv = 0,
	MediaShowFmtOpenGL,
	MediaShowFmtQT,
	MediaShowFmtOther,
	MediaShowFmtNo,
	MediaShowFmtEnd
}MediaShowFmt;

typedef enum PicFmt {
    PicFmtBmp=0,
	PicFmtYuv,
	PicFmtEnd
}PicFmt;

typedef enum RenderDataTypFmt {
    RenderDataTypFmtYuv=0,
    RenderDataTypFmtRGB,
    RenderDataTypFmtBGR,
    RenderDataTypFmtRGBA,
    RenderDataTypFmtBGRA,
    RenderDataTypFmtEnd
}RenderDataTypFmt;
#endif
