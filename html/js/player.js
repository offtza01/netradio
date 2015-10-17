var PlayerHud = function(){};
PlayerHud.prototype = {
    playingObj: null,
    playingUid: null,
    isPlaying: 0,
    volume: 0,

    updateBufforStatus: function(status) {
        this.playingObj.find('.status').text(status);
    },

    clearBufforStatus: function() {
        this.playingObj.find('.status').hide(1000);
        this.playingObj.removeClass('play').addClass('stop');
        this.isPlaying = 1;
    },

    play: function(clickedObj) {
        if( this.isPlaying)
        {
            this.stop();
        }
        this.playingObj = clickedObj;
        this.playingUid = this.playingObj.attr('data-uid');
        Player.play(this.playingUid);
    },

    stop: function() {
        this.playingObj = null;
        this.isPlaying = 0;
        Player.stop();
    },

    getClicked: function() {
        return this.playingObj;
    },

    setVolume: function(volume) {
        this.volume = volume;
        Player.setVolume(volume);
    },

    record: function(){
        if( this.isPlaying)
        {
            Player.record();
        }
    }
};
